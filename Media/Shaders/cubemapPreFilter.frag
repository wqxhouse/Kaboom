#version 420
#define NUM_FILTER_SAMPLES 1024

#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/gbufferUtil.glsl"
#include "Shaders/Material.glsl"
#include "Shaders/lightUtil.glsl"
#include "Shaders/importanceSampling.glsl"

uniform samplerCube u_cubeMapTex;
uniform int u_lod;
uniform int u_maxLodLevel;
varying vec3 v_normalWorldSpace;

vec3 PrefilterEnvMap( float roughness, vec3 R )
{
	vec3 N = R;
	vec3 V = R;

	float m = roughness * roughness;
	float m2 = m * m;
	
	vec3 prefilteredColor = vec3(0);
	float totalWeight = 0;
	
	// int numSamples = 1024 / int( u_lod + 1 );
	// int numSamples = 8192 / int( u_maxLod - u_lod );
	for(int i = 0; i < NUM_FILTER_SAMPLES; ++i)
	{
		vec2 xi  = Hammersley(i, NUM_FILTER_SAMPLES);
		vec3 H   = ImportanceSampleGGX(xi, m2, N);
		vec3 L   = 2 * dot(V, H) * H - V;
		float  NoL = saturate(dot(N, L));
		if(NoL > 0)
		{
			//vec3 lookup = fix_cube_lookup( L, u_size, u_lod );
			//prefilteredColor += textureLod( u_cubeMapTex, lookup, 0 ).xyz * NoL;
			prefilteredColor += textureLod( u_cubeMapTex, L, 0 ).xyz * NoL;
			totalWeight += NoL;
		}
	}
	
	return prefilteredColor / max(totalWeight, 0.001);
}

void main( void )
{
	vec3 normal = normalize(v_normalWorldSpace);
	vec3 color = texture( u_cubeMapTex, normal ).rgb;
	// float roughness = float(u_lod) / float(u_maxLod);
	float roughness = getRoughnessFromLod(u_lod, u_maxLodLevel);

	// TODO : check if this roughness is correct
	// color = PrefilterEnvMap( pow( roughness, 6.0f ), normal );
	color = PrefilterEnvMap( roughness, normal );
	gl_FragColor = vec4(color, 1);
	//gl_FragColor = vec4(1, 1, 1, 1);
}