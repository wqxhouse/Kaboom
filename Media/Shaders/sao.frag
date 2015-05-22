// Scalable Ambient Obscurance
// http://graphics.cs.williams.edu/papers/SAOHPG12/McGuire12SAO.pdf
// TODO: For the time being, I'll postpone the impl of hi-z mipmap optimization presented in the paper

#include "Shaders/gbufferUtil.glsl"

uniform sampler2D u_RT2; // normal + linear depth; perfect for this algorithm
uniform float u_farPlane;
// uniform vec2 u_bufferSize;
varying vec3 v_viewRay;

uniform float u_radius;
uniform float u_bias;
uniform float u_projScale; // height / (2.0 * tan(fovy * 0.5))
uniform float u_intensityDivRPow6;

uniform mat4 u_projInvMat;
// uniform sampler2D u_debugDepth;
uniform vec2 u_bufferSizeInv;

float radius2 = u_radius * u_radius;

#define NUM_SAMPLES (11)
#define NUM_SPIRAL_TURNS (7)
#define CLAMP_FAR (-500)

/////////////////////////////////////////////////////////
vec3 getViewSpacePositionV2(ivec2 ssCoord)
{
	vec4 RT2 = texelFetch(u_RT2, ssCoord, 0);
	float linearZ = recoverDepth(RT2.zw);
	
	// TODO: find a faster way 
	// 1) LUT may be an option 
	// 2) http://www.gamedev.net/topic/594142-reconstructing-view-space-position-from-depth/

	// calc viewray in screen space or sampling is not correct
	vec2 ndcXY = ((ssCoord + vec2(0.5)) * u_bufferSizeInv) * 2.0 - 1.0;
	vec4 viewRayNDCFar = vec4(ndcXY, 1, 1);
    vec4 viewRayVS = u_projInvMat * viewRayNDCFar;
    viewRayVS /= viewRayVS.w;
    viewRayVS /= viewRayVS.z;

    return getViewSpacePositionFromLinearZ(linearZ, u_farPlane, viewRayVS.xyz);

	//float z = texelFetch(u_debugDepth, ssCoord, 0).x * 2.0 - 1.0;
	//vec2 coord = ((ssCoord + vec2(0.5)) / vec2(1280, 720)) * 2.0 - 1.0;
	//vec4 posNDC = vec4(coord, z, 1.0);
	//vec4 posVS = u_projInvMat * posNDC;
	//return posVS.xyz / posVS.w;
	//return posVS;
}

void getViewSpacePositionAndNormal(ivec2 ssCoord, out vec3 viewPos, out vec3 viewNormal, out vec4 o_RT2)
{
	vec4 RT2 = texelFetch(u_RT2, ssCoord, 0);
	float linearZ = recoverDepth(RT2.zw);
    viewPos = getViewSpacePositionFromLinearZ(linearZ, u_farPlane, v_viewRay);
	viewNormal = decodeNormal(RT2.xy);
	o_RT2 = RT2;
}

/** Returns a unit vector and a screen-space radius for the tap on a unit disk (the caller should scale by the actual disk radius) */
vec2 tapLocation(int sampleNumber, float spinAngle, out float ssR)
{
    // Radius relative to ssR
    float alpha = float(sampleNumber + 0.5) * (1.0 / NUM_SAMPLES);
    float angle = alpha * (NUM_SPIRAL_TURNS * 6.28) + spinAngle;

    ssR = alpha;
    return vec2(cos(angle), sin(angle));
}

/** Read the camera-space position of the point at screen-space pixel ssP + unitOffset * ssR.  Assumes length(unitOffset) == 1 */
vec3 getOffsetPosition(ivec2 ssCoord, vec2 unitOffset, float ssR) 
{
	ivec2 ssOffsetCoord = ssCoord + ivec2(unitOffset * ssR);
	return getViewSpacePositionV2(ssOffsetCoord);
}

float sampleAO(in ivec2 ssC, in vec3 viewPos, in vec3 viewNormal, in float ssDiskRadius, 
			   in int tapIndex, in float randomPatternRotationAngle) 
{
    // Offset on the unit disk, spun for this pixel
    float ssR;
    vec2 unitOffset = tapLocation(tapIndex, randomPatternRotationAngle, ssR);
    ssR *= ssDiskRadius;
        
    // The occluding point in camera space
    vec3 offsetPosition = getOffsetPosition(ssC, unitOffset, ssR);
    vec3 v = offsetPosition - viewPos;

    float vv = dot(v, v);
    float vn = dot(v, viewNormal);

    const float epsilon = 0.01;
    
    float f = max(radius2 - vv, 0.0); 
	return f * f * f * max((vn - u_bias) / (epsilon + vv), 0.0);
}

void main() 
{
	vec3 res = vec3(0.0);

    ivec2 ssCoord = ivec2(gl_FragCoord.xy);
	vec3 viewPos;
	vec3 viewNormal;
	vec4 RT2;
	getViewSpacePositionAndNormal(ssCoord, viewPos, viewNormal, RT2);

	// optimization
	if(viewPos.z < CLAMP_FAR) discard;

	//vec2 packZ = splitFloat2x8(clamp(-viewPos.z * (1.0 / CLAMP_FAR), 0.0, 1.0));
	vec2 packZ = splitFloat2x8(clamp(viewPos.z * (1.0 / CLAMP_FAR), 0.0, 1.0));
	res.gb = packZ;

	// res.gb = RT2.zw;

    float ssDiskRadius = -u_projScale * u_radius / viewPos.z;
    float randomPatternRotationAngle = 30.0 * (ssCoord.x ^ ssCoord.y) + 10.0 * ssCoord.x * ssCoord.y;

    float sum = 0.0;
    for (int i = 0; i < NUM_SAMPLES; ++i) 
	{
        sum += sampleAO(ssCoord, viewPos, viewNormal, ssDiskRadius, i, randomPatternRotationAngle);
    }

    float AO = max(0.0, 1.0 - sum * u_intensityDivRPow6 * (5.0 / NUM_SAMPLES));

	// 2x2 box filter
    if (abs(dFdx(viewPos.z)) < 0.02) 
	{
        AO -= dFdx(AO) * ((ssCoord.x & 1) - 0.5);
    }
    if (abs(dFdy(viewPos.z)) < 0.02) 
	{
        AO -= dFdy(AO) * ((ssCoord.y & 1) - 0.5);
    }

	res.r = AO;
	gl_FragColor = vec4(res, 1);
}
