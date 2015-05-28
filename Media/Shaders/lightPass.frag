#extension GL_ARB_shader_image_load_store : enable
#pragma optionNV (unroll all)

#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/gbufferUtil.glsl"
#include "Shaders/Material.glsl"
#include "Shaders/shadowUtil.glsl"
#include "Shaders/lightUtil.glsl"

uniform sampler2D u_RT0;
uniform sampler2D u_RT1;
uniform sampler2D u_RT2;

//uniform sampler2D u_position;

// uniform sampler2D u_shadowAtlas;

uniform sampler2DShadow u_shadowAtlas; // hardware PCF
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
	ShadowDepthMap u_shadowDepthMaps[MAX_SHADOW_MAPS];
};

varying vec3 v_viewRay;
varying vec2 v_uvcoord;

uniform vec3 osg_OutputBufferSize;
// uniform vec3 u_eyePos;
uniform float u_farPlane;

uniform samplerCube u_cubeMapTex;
uniform samplerCube u_cubeMapDiffuseTex;
uniform samplerCube u_shadowFaceLookup;

uniform sampler2D u_lutTex;
uniform mat4 u_viewInvMat;

uniform int u_maxLodLevel;

void main() 
{
    // Compute texcoords
    ivec2 screenSize = textureSize(u_RT0, 0);
    ivec2 screenCoord = ivec2(gl_FragCoord.xy);

    ivec2 precomputeCoord = ivec2(vec2(screenCoord) / 
        vec2(LIGHT_TILE_SIZE_X, LIGHT_TILE_SIZE_Y) ) * 8;

    // Extract material data
    vec4 rt1 = texelFetch(u_RT1, screenCoord, 0);
	
	// optimization - as well as for skybox hack
	// mark: gbuffer clear alpha chennel to 1.0 
	// thus 0.0 represents gbuffer value
	if(rt1.a > 0.1) discard; // choose 0.1 to deal with precision

    vec4 rt0 = texelFetch(u_RT0, screenCoord, 0);
    vec4 rt2 = texelFetch(u_RT2, screenCoord, 0);

	Material material = getMaterialFromGBuffer(rt0, rt1, rt2, u_farPlane, v_viewRay);

	//material.position = texelFetch(u_position, screenCoord, 0).xyz;

    // Fetch the light counts
    // We perform a min as it *might* be that we read a wrong value
    // from the texture. Imagine reading 123123123 from the texture,
    // then the light processing loop would be 123123123 iterations long,
    // which simply crashes the driver. With this method it would be only
    // a few hundreds, which is long but does not crash the driver.
    int countPointLight = min(MAX_POINT_LIGHTS, 
        texelFetch(u_lightsPerTile, precomputeCoord + ivec2(0, 0), 0).r);

    int countPointLightShadow = min(MAX_POINT_SHADOW_LIGHTS, 
        texelFetch(u_lightsPerTile, precomputeCoord + ivec2(1, 0), 0).r);

    int countDirectionalLight = min(MAX_DIRECTIONAL_LIGHTS, 
        texelFetch(u_lightsPerTile, precomputeCoord + ivec2(2, 0), 0).r);


	int countDirectionalLightShadow = min(MAX_DIRECTIONAL_SHADOW_LIGHTS, 
        texelFetch(u_lightsPerTile, precomputeCoord + ivec2(3, 0), 0).r);

    vec3 result = vec3(0);
	result += calcEnvContribution(material, u_cubeMapDiffuseTex, u_cubeMapTex, u_lutTex, u_viewInvMat, u_maxLodLevel);

    // Compute point lights
    ivec2 baseOffset = precomputeCoord + ivec2(0, 1);
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

    // Compute shadow point lights
    baseOffset = precomputeCoord + ivec2(0, 3);
    for (int i = 0; i < countPointLightShadow; i++) 
	{
        currentOffset = ivec2(i % 8, i / 8);
        currentLightId = texelFetch(u_lightsPerTile, baseOffset + currentOffset, 0).r;
        currentLight = lights[currentLightId];

        result += applyShadowPointLight(currentLight, material, u_viewInvMat, u_shadowAtlas, u_shadowFaceLookup, u_shadowDepthMaps);
    }

    // Compute directional lights
    baseOffset = precomputeCoord + ivec2(0, 5);
    for (int i = 0; i < countDirectionalLight; i++) 
	{
        currentOffset = ivec2(i % 8, i / 8);
        currentLightId = texelFetch(u_lightsPerTile, baseOffset + currentOffset, 0).r;
        currentLight = lights[currentLightId];
        result += applyDirectionalLight(currentLight, material);
    }

    //// Compute shadowed directional lights
    baseOffset = precomputeCoord + ivec2(0, 6);
    for (int i = 0; i < countDirectionalLightShadow; i++) 
	{
        currentOffset = ivec2(i % 8, i / 8);
        currentLightId = texelFetch(u_lightsPerTile, baseOffset + currentOffset, 0).r;
        currentLight = lights[currentLightId];
        result += applyShadowDirectionalLight(currentLight, material);
    }

    // SRGB - gamma correction ( TODO: last step or here??? )
    result.xyz = pow(result.xyz, vec3(1.0 / 2.2) ); 
    //result = 1.0f - exp(-1.0 * result);

	gl_FragColor = vec4(result, 1.0);

	// debug sample atlas
	//vec3 l_ws = vec3(0, 0, -1);
	//int face = int((textureLod(u_shadowFaceLookup, l_ws, 0).r + 0.1) * 5.0);
	//int shadowMapIndex = lights[0].shadowMapIndex[face];
	//ShadowDepthMap shadowInfo = u_shadowDepthMaps[shadowMapIndex];

	//vec2 samplePos = shadowInfo.tex_scale * v_uvcoord + shadowInfo.altas_uvcoord;
	//vec4 atlasSample = textureLod(u_shadowAtlas, samplePos, 0);

	//float depth = atlasSample.r;
	//gl_FragColor = vec4(vec3(depth), 1);

	// debug vwvp transformation

	//vec4 vsPos = vec4(material.position, 1);
	//vec4 lightSpaceClip = u_shadowDepthMaps[5].vwvp * vsPos;
	//vec3 lightSpaceNDC = lightSpaceClip.xyz / lightSpaceClip.w;
	//vec3 lightSpaceNormalized = lightSpaceNDC * 0.5 + vec3(0.5);
	//float z = lightSpaceNormalized.z;

	//gl_FragColor = vec4(vec3(z), 1);

	//vec4 vsPos = vec4(material.position, 1);
	//vec4 wsPos = u_shadowDepthMaps[0].vwvp * vsPos;
	//gl_FragColor = wsPos;

	//vec3 ll = normalize(lights[0].position - material.position);
	//vec3 ws_ll = (u_viewInvMat * vec4(ll, 0)).xyz;
	//int face = int(textureLod(u_shadowFaceLookup, ws_ll, 0).r * 5.0);
	//int shadowMapIndex = lights[0].shadowMapIndex[face];
	//ShadowDepthMap shadowInfo = u_shadowDepthMaps[shadowMapIndex];

	//vec2 samplePos = shadowInfo.tex_scale * v_uvcoord + shadowInfo.altas_uvcoord;
	//vec4 atlasSample = textureLod(u_shadowAtlas, samplePos, 0);

	//float depth = atlasSample.r;
	//gl_FragColor = vec4(vec3(depth), 1);

	//gl_FragColor = vec4(ws_ll, 1);
}