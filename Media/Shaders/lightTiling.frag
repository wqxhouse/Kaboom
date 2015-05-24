#extension GL_ARB_shader_image_load_store : enable

// These uniforms need to be included before lightCulling.glsl
uniform float u_nearPlane;
uniform float u_farPlane;

// Includes
#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/gbufferUtil.glsl"
#include "Shaders/Material.glsl"
#include "Shaders/lightUtil.glsl"
#include "Shaders/lightCulling.glsl"

layout (r32i, binding=0) uniform iimage2D o_destination;
// layout (r32f, binding=0) uniform image2D o_destination;

layout(std140) uniform u_lightsBuffer
{
	// currently support up to 256 visible lights
	// due to uniform size limit & performance
	Light lights[MAX_VISIBLE_LIGHTS];
};

uniform int u_countPointLight;
uniform int u_arrayPointLight[MAX_POINT_LIGHTS];

uniform int u_countDirectionalLight;
uniform int u_arrayDirectionalLight[MAX_DIRECTIONAL_LIGHTS];

uniform int u_countShadowPointLight;
uniform int u_arrayShadowPointLight[MAX_POINT_SHADOW_LIGHTS];


//void debugFill(vec3 color)
//{
//	for(int i = 0; i < int(u_destSize.x); i++)
//	{
//		for(int j = 0; j < int(u_destSize.y); j++)
//		{
//			imageStore(o_destination, ivec2(i, j), vec4(color, 0));
//		}
//	}
//}

void debugFillTileIndex(vec3 color, int x, int y)
{
	for(int i = x * 8; i < (x+1) * 8; i++)
	{
		for(int j = y * 8; j < (y+1) * 8; j++)
		{
			// imageStore(o_destination, ivec2(i, j), vec4(color, 0));
			imageStore(o_destination, ivec2(i, j), ivec4(color, 0));
		}
	}
}

void visualizeTiledDepth(ivec2 storageCoord, float maxDepthLinear, float minDepthLinear)
{
	// visualize tiled depth
   for (int x = 0; x < 8; x+=LIGHTING_MIN_MAX_DEPTH_ACCURACY) 
	{
        for (int y = 0; y < 8; y+=LIGHTING_MIN_MAX_DEPTH_ACCURACY) 
		{
            ivec2 newCoord = storageCoord + ivec2(x, y);
			//imageStore(o_destination, newCoord, vec4(maxDepthLinear - minDepthLinear));
			imageStore(o_destination, newCoord, ivec4(maxDepthLinear - minDepthLinear));
        }
    }
}

varying vec2 v_uvcoord;
uniform sampler2D u_normalDepthTex;
// uniform sampler2D u_depthBufferTex;

//// Matrices used for the culling
uniform mat4 u_projMat;
//uniform mat4 u_viewMat;
//uniform mat4 u_viewProjMat;

uniform vec3 osg_OutputBufferSize;

const ivec2 tileSize = ivec2(LIGHT_TILE_SIZE_X, LIGHT_TILE_SIZE_Y);

//TODO: 1) change o_destination texture to integer type
//		2) disable b_computeshaderTrigger buffer color write 
void main() 
{
    // Common variables
    ivec2 screenSize = textureSize(u_normalDepthTex, 0);
    // ivec2 screenSize = textureSize(u_depthBufferTex, 0);

    // How many patches there are (e.g. 50x30 for a resolution of 1600x960)
    ivec2 precomputeSize = ivec2(osg_OutputBufferSize.xy);

    // Our patch ID (0 .. precomputeSize)
    ivec2 precomputeCoord = ivec2(v_uvcoord * precomputeSize);

    // Position on screen
    ivec2 virtualScreenSize = (precomputeSize) * tileSize;

    ivec2 screenCoord = ivec2( (gl_FragCoord.xy - 0.5) * tileSize);

    // Where to store the data
    ivec2 storageCoord = precomputeCoord * 8;

    // Compute Min and Max depth
    float minDepth = 1.0;
    float maxDepth = 0.0;

    ivec2 clampMax = ivec2(screenSize) - 1;
    ivec2 newCoord;
    float storedDepth;

    // Compute min / max depth per tile
    for (int x = 0; x < LIGHT_TILE_SIZE_X; x+=LIGHTING_MIN_MAX_DEPTH_ACCURACY) 
	{
        for (int y = 0; y < LIGHT_TILE_SIZE_Y; y+=LIGHTING_MIN_MAX_DEPTH_ACCURACY) 
		{
            newCoord = screenCoord + ivec2(x, y);

            // Check if out of screen bounds.
            // This is necessary if the precompute size is bigger than the
            // screen size. Otherwise the outer tiles will have a maxDepth of
            // 0 because they sample values which are outside of the colortex. 
            newCoord = min(newCoord, clampMax);

			vec2 sampledDepth = texelFetch(u_normalDepthTex, newCoord, 0).zw;
			storedDepth = recoverDepth(sampledDepth);

			// TODO: figure out exactly why non-linear depth buffer will make sphere test fail
			// if the camera is beyond certain distance. 
			// storedDepth = texelFetch(u_depthBufferTex, newCoord, 0).x;
            minDepth = min(minDepth, storedDepth);
            maxDepth = max(maxDepth, storedDepth);
        }
    }

/*	float minTileViewZ = -getViewZFromZ(minDepth);
	float maxTileViewZ = -getViewZFromZ(maxDepth);*/

	float minTileViewZ = minDepth * u_nearPlane;
	float maxTileViewZ = maxDepth * u_farPlane;

    // Init counters
    int processedPointLights = 0;
    int processedShadowPointLights = 0;
    int processedDirectionalLights = 0;
    //int processedShadowDirectionalLights = 0;

        // Compute tile bounds, needed for frustum
        vec2 tileScale = vec2(virtualScreenSize) * 0.5f / vec2(tileSize);
        vec2 tileBias = tileScale - vec2(precomputeCoord);

        // Build frustum
        // Based on http://gamedev.stackexchange.com/questions/67431/deferred-tiled-shading-tile-frusta-calculation-in-opengl
        // Which is based on DICE's presentation
        vec4 frustumRL = vec4(-u_projMat[0][0] * tileScale.x, 0.0f, tileBias.x, 0.0f);
        vec4 frustumTL = vec4(0.0f, -u_projMat[1][1] * tileScale.y, tileBias.y, 0.0f);

        const vec4 frustumOffset = vec4(0.0f, 0.0f, -1.0f, 0.0f);

		// debug
		FrustumDebug fd;
		{
			fd.frustumPlanes[0] = frustumOffset - frustumRL;
			fd.frustumPlanes[1] = frustumOffset + frustumRL;
			fd.frustumPlanes[2] = frustumOffset - frustumTL;
			fd.frustumPlanes[3] = frustumOffset + frustumTL;
			fd.frustumPlanes[4] = vec4(0.0, 0.0, -1.0, -minTileViewZ);
			fd.frustumPlanes[5] = vec4(0.0, 0.0, 1.0, maxTileViewZ);
		}

		// debug, normalize
		for (int i = 0; i < 4; ++i) 
		{
			fd.frustumPlanes[i] *= 1.0 / length(fd.frustumPlanes[i].xyz);
		}

        // Buffer layout:
        // First 8 pixels store count
        // Second/Third line stores point lights
        // Fourth/Fifth line stores shadow point lights
        // Sixth stores directional lights
        // Seventh stores directional shadowed lights
        // Eight is free (yet)

        // Process Point Lights
        ivec2 baseOffset = storageCoord + ivec2(0,1);
        ivec2 currentOffset = ivec2(0);

        for (int i = 0; i < u_countPointLight; i++) 
		{
            int index = u_arrayPointLight[i];
            Light light = lights[index];
            //if (isPointLightInFrustum(light, frustum)) 
            if (isPointLightInFrustumDebug(light, fd)) 
			{
                currentOffset = ivec2(processedPointLights % 8, processedPointLights / 8);
                imageStore(o_destination, baseOffset + currentOffset, ivec4(index));
                processedPointLights += 1;
            }
        }

        // Process shadowed point lights
        baseOffset = storageCoord + ivec2(0,3);
        currentOffset = ivec2(0);

        for (int i = 0; i < u_countShadowPointLight; i++) {
            int index = u_arrayShadowPointLight[i];
            Light light = lights[index];
            if (isPointLightInFrustumDebug(light, fd)) {
                currentOffset = ivec2(processedShadowPointLights % 8, processedShadowPointLights / 8);
                imageStore(o_destination, baseOffset + currentOffset, ivec4(index));
                processedShadowPointLights += 1;
            }
        }

        // Process directional lights
        baseOffset = storageCoord + ivec2(0,5);
        currentOffset = ivec2(0);

        for (int i = 0; i < u_countDirectionalLight; i++) 
		{
            // No frustum check. Directional lights are always visible
            int index = u_arrayDirectionalLight[i];
            Light light = lights[index];
            currentOffset = ivec2(processedDirectionalLights % 8, processedDirectionalLights / 8);
            imageStore(o_destination, baseOffset + currentOffset, ivec4(index));
            processedDirectionalLights += 1;
        }

        // Process shadowed directional lights
        //baseOffset = storageCoord + ivec2(0,6);
        //currentOffset = ivec2(0);

        //for (int i = 0; i < countDirectionalLightShadow; i++) {
        //    // No frustum check. Directional lights are always visible
        //    int index = arrayDirectionalLightShadow[i];
        //    Light light = lights[index];
        //    currentOffset = ivec2(processedDirectionalShadowLights % 8, processedDirectionalShadowLights / 8);
        //    imageStore(destination, baseOffset + currentOffset, ivec4(index));
        //    processedDirectionalShadowLights += 1;
        //}

	//gl_FragColor = vec4(minDepth);

    imageStore(o_destination, storageCoord + ivec2(0, 0), ivec4(processedPointLights));
    imageStore(o_destination, storageCoord + ivec2(1, 0), ivec4(processedShadowPointLights));
    imageStore(o_destination, storageCoord + ivec2(2, 0), ivec4(processedDirectionalLights));
//    imageStore(o_destination, storageCoord + ivec2(3, 0), ivec4(processedDirectionalShadowLights));
}

