#version 420
#define NUM_FILTER_SAMPLES 1024

#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/gbufferUtil.glsl"
#include "Shaders/Material.glsl"
#include "Shaders/lightUtil.glsl"
#include "Shaders/importanceSampling.glsl"

#pragma optionNV (unroll none) // hopefully shorten the shader time

uniform samplerCube u_cubeMapTex;
uniform int u_lod;
uniform int u_maxLodLevel;
varying vec3 v_normalWorldSpace;

vec3 PrefilterEnvMapDiffuse( vec3 N )
{
	vec3 diffuse = vec3(0.0);
	const int diffuseSampleNum = 1024;
	for( int i = 0; i < diffuseSampleNum; i++ )
	{
		vec2 E = Hammersley(i, diffuseSampleNum);
		vec3 L = ImportanceSampleDiffuse( E, N );

		float NoL = dot( N, L );
		if( NoL > 0 )
		{
			vec3 LL = normalize(2 * NoL * L - N);
			float NoLL = saturate(dot(N, LL));
			
			float pdf = NoLL * INV_M_PI;
			// TODO: currently hard coded, change to dynamic
			const int cubeTexWidth = 1024;

            float solidAngleTexel = 4 * M_PI / (6 * cubeTexWidth * cubeTexWidth);
            float solidAngleSample = 1.0 / (diffuseSampleNum * pdf);
            float lod = 0.5 * log2(float(solidAngleSample / solidAngleTexel));

			vec3 SampleColor = textureLod(u_cubeMapTex, L, lod).rgb;
			diffuse += SampleColor;
		}
	}

	return diffuse / diffuseSampleNum;
}

void main( void )
{
	vec3 normal = normalize(v_normalWorldSpace);
	vec3 color = PrefilterEnvMapDiffuse( normal );
	gl_FragColor = vec4(color, 1);
}