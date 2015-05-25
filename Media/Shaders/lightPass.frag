#extension GL_ARB_shader_image_load_store : enable

#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/gbufferUtil.glsl"
#include "Shaders/Material.glsl"
#include "Shaders/lightUtil.glsl"
#include "Shaders/shadowUtil.glsl"

#pragma optionNV (unroll all)

uniform sampler2D u_RT0;
uniform sampler2D u_RT1;
uniform sampler2D u_RT2;

uniform isampler2D u_lightsPerTile;
//uniform sampler2D u_lightsPerTile;

layout(std140) uniform u_lightsBuffer
{
	// currently support up to 128 visible lights
	// due to uniform size limit & performance
	Light lights[MAX_VISIBLE_LIGHTS];
};

layout(std140) uniform u_shadowDepthMapBuffer 
{
	ShadowDepthMap shadowDepthMap[MAX_SHADOW_MAPS];
};

varying vec2 v_uvcoord;
varying vec3 v_viewRay;

uniform vec3 osg_OutputBufferSize;
// uniform vec3 u_eyePos;
uniform float u_farPlane;

uniform samplerCube u_cubeMapTex;
uniform samplerCube u_cubeMapDiffuseTex;
uniform sampler2D u_lutTex;
uniform mat4 u_viewInvMat;

uniform int u_maxLodLevel;

void main() 
{
	// vec4 abc = vec4(shadowDepthMap[0].altas_uvcoord, 0, 1);
	// return;

    // Compute texcoords
    ivec2 screenSize = textureSize(u_RT0, 0);

    ivec2 screenCoord = ivec2(gl_FragCoord.xy);
    vec2 correctTexcoord = v_uvcoord;

    ivec2 precomputeCoord = ivec2(vec2(screenCoord) / 
        vec2(LIGHT_TILE_SIZE_X, LIGHT_TILE_SIZE_Y) ) * 8;

    // Extract material data
    vec4 rt1 = texelFetch(u_RT1, screenCoord, 0);
	
	// optimization - as well as for skybox hack
	// mark: gbuffer clear alpha chennel to 1.0 
	// thus 0.0 represents gbuffer value
	if(rt1.a > 0.1 ) discard; // choose 0.1 to deal with precision

    vec4 rt0 = texelFetch(u_RT0, screenCoord, 0);
    vec4 rt2 = texelFetch(u_RT2, screenCoord, 0);

	Material material = getMaterialFromGBuffer(rt0, rt1, rt2, u_farPlane, v_viewRay);

    // Fetch the light counts
    // We perform a min as it *might* be that we read a wrong value
    // from the texture. Imagine reading 123123123 from the texture,
    // then the light processing loop would be 123123123 iterations long,
    // which simply crashes the driver. With this method it would be only
    // a few hundreds, which is long but does not crash the driver.
    int countPointLight = min(MAX_POINT_LIGHTS, 
        texelFetch(u_lightsPerTile, precomputeCoord + ivec2(0,0), 0).r);

    //int countPointLightShadow = min(MAX_SHADOW_POINT_LIGHTS, 
    //    texelFetch(lightsPerTile, precomputeCoord + ivec2(1,0), 0).r);

    int countDirectionalLight = min(MAX_DIRECTIONAL_LIGHTS, 
        texelFetch(u_lightsPerTile, precomputeCoord + ivec2(2,0), 0).r);

    //int countDirectionalLightShadow = min(MAX_SHADOW_POINT_LIGHTS, 
    //    texelFetch(lightsPerTile, precomputeCoord + ivec2(3,0), 0).r);

    vec3 result = vec3(0);
	result += calcEnvContribution(material, u_cubeMapDiffuseTex, u_cubeMapTex, u_lutTex, u_viewInvMat, u_maxLodLevel);

    // Compute point lights
    ivec2 baseOffset = precomputeCoord + ivec2(0,1);
    ivec2 currentOffset = ivec2(0);
    int currentLightId = 0;
    Light currentLight;

    for (int i = 0; i < countPointLight; i++) 
	{
        currentOffset = ivec2(i % 8, i / 8);
        currentLightId = texelFetch(u_lightsPerTile, baseOffset + currentOffset, 0).r;
        currentLight = lights[currentLightId];

        result += applyPointLight(currentLight, material);
    }

    //// Compute shadow point lights
    //baseOffset = precomputeCoord + ivec2(0,3);
    //for (int i = 0; i < countPointLightShadow; i++) {
    //    currentOffset = ivec2(i % 8, i / 8);
    //    currentLightId = texelFetch(lightsPerTile, baseOffset + currentOffset, 0).r;
    //    currentLight = lights[currentLightId];

    //    #if USE_SHADOWS
    //        result += applyPointLightWithShadow(currentLight, material OCCLUSION_PER_LIGHT_SEND_PARAMETERS );
    //    #else
    //        result += applyPointLight(currentLight, material OCCLUSION_PER_LIGHT_SEND_PARAMETERS );
    //    #endif
    //}

    // Compute directional lights
    baseOffset = precomputeCoord + ivec2(0,5);
    for (int i = 0; i < countDirectionalLight; i++) 
	{
        currentOffset = ivec2(i % 8, i / 8);
        currentLightId = texelFetch(u_lightsPerTile, baseOffset + currentOffset, 0).r;
        currentLight = lights[currentLightId];
        result += applyDirectionalLight(currentLight, material);
    }

	//result = material.baseColor;
	//result = lights[1].color;


    //// Compute shadowed directinal lights
    //baseOffset = precomputeCoord + ivec2(0,6);
    //for (int i = 0; i < countDirectionalLightShadow; i++) {
    //    currentOffset = ivec2(i % 8, i / 8);
    //    currentLightId = texelFetch(lightsPerTile, baseOffset + currentOffset, 0).r;
    //    currentLight = lights[currentLightId];
    //    result += applyDirectionalLightWithShadow(currentLight, material OCCLUSION_PER_LIGHT_SEND_PARAMETERS );
    //}


    // SRGB - gamma correction ( TODO: last step or here??? )
    result.xyz = pow(result.xyz, vec3(1.0 / 2.2) ); 
    //result = 1.0f - exp(-1.0 * result);

	gl_FragColor = vec4(result, 1.0);
}