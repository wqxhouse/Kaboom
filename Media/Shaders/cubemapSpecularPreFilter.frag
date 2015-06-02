#version 420
#define NUM_FILTER_SAMPLES 1024

#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/gbufferUtil.glsl"
#include "Shaders/Material.glsl"
#include "Shaders/shadowUtil.glsl"
#include "Shaders/lightUtil.glsl"
#include "Shaders/importanceSampling.glsl"

#pragma optionNV (unroll none) // hopefully shorten the shader time

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
	
	for(int i = 0; i < NUM_FILTER_SAMPLES; ++i)
	{
		vec2 xi  = Hammersley(i, NUM_FILTER_SAMPLES);
		vec3 H   = ImportanceSampleGGX(xi, m2, N);
		vec3 L   = 2 * dot(V, H) * H - V;
		float  NoL = saturate(dot(N, L));
		if(NoL > 0)
		{
			prefilteredColor += textureLod( u_cubeMapTex, L, 0 ).xyz * NoL;
			totalWeight += NoL;
		}
	}
	
	return prefilteredColor / max(totalWeight, 0.001);
}

void main( void )
{
	vec3 normal = normalize(v_normalWorldSpace);
	// vec3 color = texture( u_cubeMapTex, normal ).rgb;
	float roughness = getRoughnessFromLod(u_lod, u_maxLodLevel);
	vec3 color = PrefilterEnvMap( roughness, normal );
	gl_FragColor = vec4(color, 1);
}