#extension GL_ARB_shader_image_load_store : enable

// Includes
#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/lightUtil.glsl"
#include "Shaders/gbufferUtil.glsl"

//layout (r32i) uniform iimage2D o_destination;
layout (r32f, binding=0) uniform image2D o_destination;

//layout(std140) uniform u_lightsBuffer
//{
//	// currently support up to 256 lights
//	Light lights[MAX_VISIBLE_LIGHTS];
//}

void main()
{
	//if(gl_FragCoord.x - 0.5 > 0) 
	//{
	//	// work around texture minimum limitation
	//	gl_FragColor = vec4(0.1, 0, 0, 0);
	//	return;
	//}

	//imageStore(o_destination, storageCoord + ivec2(0, 0), ivec4(processedPointLights));
	gl_FragColor = vec4(1, 0, 0, 0);
}

//in vec2 texcoord;

//uniform sampler2D u_normalDepthTex;

//layout (r32i) uniform iimage2D o_destination;

//// Per-Light data and count
//layout(std140) uniform  u_lightsBuffer
//{
//	// currently support up to 256 lights
//	Light lights[MAX_VISIBLE_LIGHTS];
//}

//uniform int u_countDirectionalLight;
//uniform int u_arrayDirectionalLight[MAX_DIRECTIONAL_LIGHTS];

//uniform int u_countPointLight;
//uniform int u_arrayPointLight[MAX_POINT_LIGHTS];

//// Matrices used for the culling
//uniform mat4 u_projMat;
//uniform mat4 u_modelViewMat;

//// TODO: test buffer size impl
//uniform vec3 osg_OutputBufferSize;

//const ivec2 patchSize = ivec2(LIGHT_TILE_SIZE_X, LIGHT_TILE_SIZE_Y);


//void main() {

//    // Common variables
//    ivec2 screenSize = textureSize(u_normalDepthTex, 0);

//    // How many patches there are (e.g. 50x30 for a resolution of 1600x960)
//    ivec2 precomputeSize = ivec2(osg_OutputBufferSize.xy);

//    // Our patch ID (0 .. precomputeSize)
//    ivec2 precomputeCoord = ivec2(texcoord * precomputeSize);

//    // Position on screen
//    ivec2 virtualScreenSize = (precomputeSize) * patchSize;

//    ivec2 screenCoord = ivec2( (gl_FragCoord.xy - 0.5) * patchSize);

//    // Where to store the data
//    ivec2 storageCoord = precomputeCoord * 8;

//    // Compute Min and Max depth
//    float minDepth = 1.0;
//    float maxDepth = 0.0;

//    ivec2 clampMax = ivec2(screenSize) - 1;
//    ivec2 newCoord;
//    float storedDepth;

//    // vec2 texelSize = 1.0 / screenSize;
//    // vec2 screenCoordTexel = vec2(newCoord) / screenSize;
//    // vec2 newCoordTexel;

//    // Compute min / max depth per tile
//    for (int x = 0; x < LIGHTING_COMPUTE_PATCH_SIZE_X; x+=LIGHTING_MIN_MAX_DEPTH_ACCURACY*2) 
//	{
//        for (int y = 0; y < LIGHTING_COMPUTE_PATCH_SIZE_Y; y+=LIGHTING_MIN_MAX_DEPTH_ACCURACY) 
//		{
//            newCoord = screenCoord + ivec2(x + y%2,y);
//            // newCoordTexel = vec2(x,y) * texelSize + screenCoordTexel;

//            // Check if out of screen bounds.
//            // This is necessary if the precompute size is bigger than the
//            // screen size. Otherwise the outer tiles will have a maxDepth of
//            // 0 because they sample values which are outside of the colortex. 
//            newCoord = min(newCoord, clampMax);

//			vec2 sampledNormalDepth = texelFetch(u_normalDepthTex, newCoord, 0).zw;
//			storedDepth = recoverDepth(sampledDepth);
//            // storedDepth = texelFetch(depth, newCoord, 0).r;


//            minDepth = min(minDepth, storedDepth);
//            maxDepth = max(maxDepth, storedDepth);

//        }
//    }

//    // Also store linear depth to be able to compare
//    // light depth and min/max depth easily
//    //float minDepthLinear = getLinearZFromZ(minDepth);
//    // float maxDepthLinear = getLinearZFromZ(maxDepth);

//    // Init counters
//    int processedPointLights = 0;
//    int processedShadowPointLights = 0;
//    int processedDirectionalLights = 0;
//    int processedDirectionalShadowLights = 0;

//        // Compute tile bounds, needed for frustum
//        vec2 tileScale = vec2(virtualScreenSize) * 0.5f / vec2(patchSize);
//        vec2 tileBias = tileScale - vec2(precomputeCoord);

//        // Store matrices in local variables as their name
//        // can get very long
//        mat4 projMat = PROJ_MAT;
//        mat4 viewMat = VIEW_MAT;
//        mat4 mvpMat = MODEL_VIEW_PROJ_MAT;

//        // Build frustum
//        // Based on http://gamedev.stackexchange.com/questions/67431/deferred-tiled-shading-tile-frusta-calculation-in-opengl
//        // Which is based on DICE's presentation
//        vec4 frustumRL = vec4(-projMat[0][0] * tileScale.x, 0.0f, tileBias.x, 0.0f);
//        vec4 frustumTL = vec4(0.0f, -projMat[1][1] * tileScale.y, tileBias.y, 0.0f);

//        // The doc said frustumOffset = vec4(0,0,1,0) but panda uses 
//        // apparently an inverted coordinate system
//        const vec4 frustumOffset = vec4(0.0f, 0.0f, -1.0f, 0.0f);

//        // Calculate frustum planes
//        Frustum frustum;
//        frustum.left   = normalize(frustumOffset - frustumRL);
//        frustum.right  = normalize(frustumOffset + frustumRL);
//        frustum.top    = normalize(frustumOffset - frustumTL);
//        frustum.bottom = normalize(frustumOffset + frustumTL);
//        frustum.near   = minDepth;
//        frustum.far    = maxDepth;

//        frustum.viewMat = viewMat;
//        frustum.mvpMat  = mvpMat;

//        // Buffer layout:
//        // First 8 pixels store count
//        // Second/Third line stores point lights
//        // Fourth/Fifth line stores shadow point lights
//        // Sixt stores directional lights
//        // Seventh stores directional shadowed lights
//        // Eight is free (yet)

//        // Process Point Lights
//        ivec2 baseOffset = storageCoord + ivec2(0,1);
//        ivec2 currentOffset = ivec2(0);

//        for (int i = 0; i < countPointLight; i++) {
//            int index = arrayPointLight[i];
//            Light light = lights[index];
//            if (isPointLightInFrustum(light, frustum)) {
//                currentOffset = ivec2(processedPointLights % 8, processedPointLights / 8);
//                imageStore(destination, baseOffset + currentOffset, ivec4(index));
//                processedPointLights += 1;
//            }
//        }

//        // Process shadowed point lights
//        baseOffset = storageCoord + ivec2(0,3);
//        currentOffset = ivec2(0);

//        for (int i = 0; i < countPointLightShadow; i++) {
//            int index = arrayPointLightShadow[i];
//            Light light = lights[index];
//            if (isPointLightInFrustum(light, frustum)) {
//                currentOffset = ivec2(processedShadowPointLights % 8, processedShadowPointLights / 8);
//                imageStore(destination, baseOffset + currentOffset, ivec4(index));
//                processedShadowPointLights += 1;
//            }
//        }

//        // Process directional lights
//        baseOffset = storageCoord + ivec2(0,5);
//        currentOffset = ivec2(0);

//        for (int i = 0; i < countDirectionalLight; i++) {
//            // No frustum check. Directional lights are always visible
//            int index = arrayDirectionalLight[i];
//            Light light = lights[index];
//            currentOffset = ivec2(processedDirectionalLights % 8, processedDirectionalLights / 8);
//            imageStore(destination, baseOffset + currentOffset, ivec4(index));
//            processedDirectionalLights += 1;
//        }


//        // Process shadowed directional lights
//        baseOffset = storageCoord + ivec2(0,6);
//        currentOffset = ivec2(0);

//        for (int i = 0; i < countDirectionalLightShadow; i++) {
//            // No frustum check. Directional lights are always visible
//            int index = arrayDirectionalLightShadow[i];
//            Light light = lights[index];
//            currentOffset = ivec2(processedDirectionalShadowLights % 8, processedDirectionalShadowLights / 8);
//            imageStore(destination, baseOffset + currentOffset, ivec4(index));
//            processedDirectionalShadowLights += 1;
//        }
//    }

//    imageStore(destination, storageCoord + ivec2(0, 0), ivec4(processedPointLights));
//    imageStore(destination, storageCoord + ivec2(1, 0), ivec4(processedShadowPointLights));
//    imageStore(destination, storageCoord + ivec2(2, 0), ivec4(processedDirectionalLights));
//    imageStore(destination, storageCoord + ivec2(3, 0), ivec4(processedDirectionalShadowLights));
//}