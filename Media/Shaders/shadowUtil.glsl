#include "Shaders/Samples.glsl"

struct ShadowDepthMap
{
	mat4 vwvp;
	vec2 altas_uvcoord;
	float tex_scale;
};

vec2 getShadowOffsets(vec3 N, vec3 L) 
{
    float cos_alpha = clamp(dot(N, L), 0.0, 1.0);
    float offset_scale_N = sqrt(1 - cos_alpha*cos_alpha); 
    float offset_scale_L = offset_scale_N / cos_alpha;   
    return vec2(offset_scale_N, min(2, offset_scale_L));
}

vec3 computeBiasedPosition(vec3 position, float slopeBias, float normalBias, vec3 l, vec3 n) 
{
    float baseShift = 1.0;
    vec2 offsets = getShadowOffsets(n, l);
    position += baseShift * l * offsets.x * slopeBias;
    position += baseShift * n * offsets.y * normalBias; 
    return position;
}

vec3 reprojectShadow(ShadowDepthMap shadowInfo, vec3 pos) 
{
    vec4 projected = shadowInfo.vwvp * vec4(pos, 1);
    return (projected.xyz / projected.w * 0.5) + 0.5;
}

vec2 calcAtlasUVCoord(vec2 fullQuadCoord, ShadowDepthMap shadowInfo) 
{
    return clamp(fullQuadCoord, 0.0, 1.0) * shadowInfo.tex_scale + shadowInfo.altas_uvcoord;
}

float PCF(sampler2DShadow u_shadowAtlas, ShadowDepthMap shadowInfo, vec3 projCoord, float baseBias, vec2 projSize) 
{
	// TODO: find a more efficient way to deal with out of bounds problem without comparsion
	if(projCoord.x > 1.0 || projCoord.y > 1.0 || projCoord.x < 0.0 || projCoord.y < 0.0) 
	{
		return 1.0;
	}	

    vec2 atlasCoord = calcAtlasUVCoord(projCoord.xy, shadowInfo);
    float biasedDepth = projCoord.z - baseBias;
    vec2 filterRadiusUV = 2.0 * projSize;

    // PCF Filter
    float sum = 0.0;
    for ( int i = 0; i < 16; ++i ) // 16 samples
    {
		vec2 offset = poissonDisk32[i] * filterRadiusUV;
        sum += 1.0 - textureLod(u_shadowAtlas, vec3(atlasCoord + offset, biasedDepth) , 0);

		//float sampled = textureLod(u_shadowAtlas, atlasCoord + offset , 0).x;
		//sum += step(sampled, biasedDepth);        
    }

    sum /= 16;
    return 1.0 - clamp(sum, 0.0, 1.0);
}

float PCFGather(sampler2DShadow u_shadowAtlas, ShadowDepthMap shadowInfo, vec3 projCoord, float baseBias, vec2 projSize)
{
	if(projCoord.x > 1.0 || projCoord.y > 1.0 || projCoord.x < 0.0 || projCoord.y < 0.0) 
	{
		return 1.0;
	}	

    vec2 atlasCoord = calcAtlasUVCoord(projCoord.xy, shadowInfo);
    float biasedDepth = projCoord.z - baseBias;
	vec2 offset = fract(projCoord.xy - 0.5);
	vec4 group1 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(-2, -2));
	vec4 group2 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(0, -2));
	vec4 group3 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(2, -2));
	vec4 group4 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(-2, 0));
	vec4 group5 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(0, 0));
	vec4 group6 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(2, 0));
	vec4 group7 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(-2, 2));
	vec4 group8 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(0, 2));
	vec4 group9 = textureGatherOffset(u_shadowAtlas, atlasCoord, biasedDepth, ivec2(2, 2));
	vec4 locols = vec4(group1.ab, group3.ab);
	vec4 hicols = vec4(group7.rg, group9.rg);
	locols.yz += group2.ab;
	hicols.yz += group8.rg;
	vec4 midcols = vec4(group1.rg, group3.rg) +  vec4(group7.ab, group9.ab) +
                   vec4(group4.rg, group6.rg) + vec4(group4.ab, group6.ab) +
				   mix(locols, hicols, offset.y);
	vec4 cols = group5 + vec4(group2.rg, group8.ab);
                    cols.xyz += mix(midcols.xyz, midcols.yzw, offset.x);
	return dot(cols, vec4(1.0/25.0));
}

float computePointLightShadow(sampler2DShadow u_shadowAtlas, ShadowDepthMap shadowInfo, vec3 position,
			vec3 n, vec3 l, float slopeScaledBias, float normalScaledBias, float baseBias) 
{
    vec3 biasedPos = computeBiasedPosition(position, slopeScaledBias, normalScaledBias, n, l); 
	vec3 projCoord = reprojectShadow(shadowInfo, biasedPos);

    return PCFGather(u_shadowAtlas, shadowInfo, projCoord, baseBias, vec2(0.5 / SHADOW_MAP_ATLAS_SIZE) );
}

void getPSSMCascade(ShadowDepthMap depthMap[MAX_SHADOW_MAPS], int lightSMIndices[6],  
		vec3 position, float borderFactor, out vec3 projCoord, out ShadowDepthMap selectedShadowInfo) 
{
	// hard coded for 3 now
    for (int i = 0; i < 3; i++) 
	{
        int shadowMapIndex = lightSMIndices[i];
        // ShadowDepthMap shadowInfo = depthMap[shadowMapIndex]
		selectedShadowInfo = depthMap[shadowMapIndex];
        projCoord = reprojectShadow(selectedShadowInfo, position);

        if (all(greaterThan(projCoord.xy, vec2(borderFactor))) 
		 && all(lessThan(projCoord.xy, vec2(1-borderFactor)))) 
		{
            // Source can be used
            break;
        }
    }
}

/* TODO: change back to pssm if have time
float computeDirectionalLightShadow(sampler2DShadow u_shadowAtlas, ShadowDepthMap depthMap[MAX_SHADOW_MAPS],
	int lightSMIndices[6], vec3 position, vec3 n, vec3 l, 
    float slopeScaledBias, float normalScaledBias, float baseBias) 
{
    vec3 projCoord;
	ShadowDepthMap selectedShadowInfo;
    getPSSMCascade(depthMap, lightSMIndices, position, 0.0, projCoord, selectedShadowInfo);
    vec3 biasedPos = computeBiasedPosition(position, slopeScaledBias / 1024.0, 
			normalScaledBias / 1024.0, n, l); // hard code resolution 
    projCoord = reprojectShadow(selectedShadowInfo, biasedPos);
    baseBias /= 1024.0;

	return PCF(u_shadowAtlas, selectedShadowInfo, projCoord, baseBias, vec2(0.5 / SHADOW_MAP_ATLAS_SIZE));
}
*/

float computeDirectionalLightShadow(sampler2DShadow u_shadowAtlas, ShadowDepthMap depthMap[MAX_SHADOW_MAPS],
	int lightSMIndices[6], vec3 position, vec3 n, vec3 l, 
    float slopeScaledBias, float normalScaledBias, float baseBias)
{
	const float resInv = 1.0 / 2048.0;
	vec3 biasedPos = computeBiasedPosition(position, slopeScaledBias * resInv, 
			normalScaledBias * resInv, n, l); // hard code resolution  
	ShadowDepthMap sdm = depthMap[lightSMIndices[0]];
	vec3 projCoord = reprojectShadow(sdm, biasedPos);
	baseBias *= resInv;

	return PCF(u_shadowAtlas, sdm, projCoord, baseBias, vec2(0.5 / SHADOW_MAP_ATLAS_SIZE));
}

float computeDirectionalLightShadowMask(sampler2DShadow u_shadowAtlas, vec3 position, vec3 n, vec3 l, 
	mat4 vwvp, vec2 atlasUVCoord, float texScale,
    float slopeScaledBias, float normalScaledBias, float baseBias)
{
	const float resInv = 1.0 / 2048.0;
	vec3 biasedPos = computeBiasedPosition(position, slopeScaledBias * resInv, 
			normalScaledBias * resInv, n, l); // hard code resolution  
	// vec3 biasedPos = computeBiasedPosition(position, slopeScaledBias, normalScaledBias, n, l); 

	ShadowDepthMap sdm;
	sdm.vwvp = vwvp;
	sdm.altas_uvcoord = atlasUVCoord;
	sdm.tex_scale = texScale;

	vec3 projCoord = reprojectShadow(sdm, biasedPos);
	baseBias *= resInv;

	return PCFGather(u_shadowAtlas, sdm, projCoord, baseBias, vec2(0.5 / SHADOW_MAP_ATLAS_SIZE));
}