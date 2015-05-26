struct ShadowDepthMap
{
	mat4 vwvp;
	vec2 altas_uvcoord;
	float tex_scale;
};

vec2 getShadowOffsets(vec3 N, vec3 L) 
{
    float cos_alpha = clamp(dot(N, L), 0.0, 1.0);
    float offset_scale_N = sqrt(1 - cos_alpha*cos_alpha); // sin(acos(L¡¤N))
    float offset_scale_L = offset_scale_N / cos_alpha;    // tan(acos(L¡¤N))
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

vec2 calcAtlasUVCoord(vec2 fullQuadCoord, in ShadowDepthMap shadowInfo) 
{
    return clamp(fullQuadCoord, 0, 1) * shadowInfo.tex_scale + shadowInfo.altas_uvcoord;
}

float PCF(sampler2D u_shadowAtlas, ShadowDepthMap shadowInfo, vec3 projCoord, float baseBias, vec2 projSize) 
{
    vec2 atlasCoord = calcAtlasUVCoord(projCoord.xy, shadowInfo);
    float biasedDepth = projCoord.z - baseBias;
    vec2 filterRadiusUV = 2.0 * projSize;

    // PCF Filter
    float sum = 0.0;
    for ( int i = 0; i < 16; ++i ) // 16 samples
    {
		vec2 offset = poissonDisk32[i] * filterRadiusUV;
            //sum += 1.0 - textureLod(shadowAtlasPCF, vec3(centerCoord + offset, biasedDepth) , 0);

		float sampled = textureLod(u_shadowAtlas, atlasCoord + offset , 0).x;
		sum += step(sampled, biasedDepth);        
    }

    sum /= 16;
    return 1.0 - clamp(sum, 0.0, 1.0);
}

float computePointLightShadow(sampler2D u_shadowAtlas, ShadowDepthMap shadowInfo, float slopeScaledBias, 
							  float normalScaledBias, float baseBias) 
{
    vec3 biasedPos = computeBiasedPosition(material.position, slopeScaledBias, normalScaledBias, n, l);
    vec3 projCoord = reprojectShadow(shadowInfo, biasedPos);

    return PCF(u_shadowAtlas, shadowInfo, projCoord, baseBias, vec2(0.5 / SHADOW_MAP_ATLAS_SIZE) );
}