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

uniform sampler2D u_position;
uniform sampler2D u_normal;

float radius2 = u_radius * u_radius;

// uniform uniform2D u_debugDepth;

//#define NUM_SAMPLES (11)
//#define NUM_SPIRAL_TURNS (7)

#define NUM_SAMPLES (11)
#define NUM_SPIRAL_TURNS (7)
#define CLAMP_FAR (-500)



/////////////////////////////////////////////////////////
vec3 getViewSpacePosition(ivec2 ssCoord)
{
	vec4 RT2 = texelFetch(u_RT2, ssCoord, 0);
	float linearZ = recoverDepth(RT2.zw);
    return getViewSpacePositionFromLinearZ(linearZ, u_farPlane, v_viewRay);
}

void getViewSpacePositionAndNormal(ivec2 ssCoord, out vec3 viewPos, out vec3 viewNormal, out vec4 o_RT2)
{
/*
	vec4 RT2 = texelFetch(u_RT2, ssCoord, 0);
	float linearZ = recoverDepth(RT2.zw);
    viewPos = getViewSpacePositionFromLinearZ(linearZ, u_farPlane, v_viewRay);
	viewNormal = decodeNormal(RT2.xy);
	o_RT2 = RT2;
*/
/*
	vec2 coord = vec2(gl_FragCoord.xy) / vec2(1280, 720);
	vec4 RT2 = texture(u_RT2, coord);
	float linearZ = recoverDepth(RT2.zw);
	viewPos = getViewSpacePositionFromLinearZ(linearZ, u_farPlane, v_viewRay);
	viewNormal = decodeNormal(RT2.xy);
	*/
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

vec3 getPosition(ivec2 uv)
{
	return texelFetch(u_position, uv, 0).xyz;
}


/** Read the camera-space position of the point at screen-space pixel ssP + unitOffset * ssR.  Assumes length(unitOffset) == 1 */
vec3 getOffsetPosition(ivec2 ssCoord, vec2 unitOffset, float ssR) 
{
	ivec2 ssOffsetCoord = ssCoord + ivec2(unitOffset * ssR);
	// vec3 pos = getViewSpacePosition(ssOffsetCoord);
/*
	vec2 coord = gl_FragCoord.xy / vec2(1280, 720);
	vec4 RT2 = texture(u_RT2, coord);
	float linearZ = recoverDepth(RT2.zw);
    vec3 pos = getViewSpacePositionFromLinearZ(linearZ, u_farPlane, v_viewRay);


	return pos;
	*/
	return getPosition(ssOffsetCoord);
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

vec3 getNormal(ivec2 coord)
{
	//return normalize(cross(dFdy(C), dFdx(C)));
	return texelFetch(u_normal, coord, 0).xyz;
	//return vec3(1);
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() 
{
	vec3 res = vec3(0.0);

    ivec2 ssCoord = ivec2(gl_FragCoord.xy);


	vec3 viewPos;
	vec3 viewNormal;
	vec4 RT2;
	getViewSpacePositionAndNormal(ssCoord, viewPos, viewNormal, RT2);

	float recoveredZ = recoverDepth(RT2.zw);

	viewPos = getPosition(ssCoord);
	viewNormal = getNormal(ssCoord);

	// optimization
	if(viewPos.z < CLAMP_FAR) discard;

	//vec2 packZ = splitFloat2x8(clamp(-viewPos.z * (1.0 / CLAMP_FAR), 0.0, 1.0));
	vec2 packZ = splitFloat2x8(clamp(viewPos.z * (1.0 / CLAMP_FAR), 0.0, 1.0));
	res.gb = packZ;

	//res.g = clamp(viewPos.z / CLAMP_FAR, 0.0, 1.0);

	//res.gb = RT2.zw;
	//res.gb = splitDepth2x16(clamp(-viewPos.z / 500.0, 0.0, 1.0));

	//res.g = -viewPos.z / u_farPlane;

    float ssDiskRadius = -u_projScale * u_radius / viewPos.z;
    float randomPatternRotationAngle = 30.0 * (ssCoord.x ^ ssCoord.y) + 10.0 * ssCoord.x * ssCoord.y;
	//float randomPatternRotationAngle = rand(ssCoord);

    float sum = 0.0;
    for (int i = 0; i < NUM_SAMPLES; ++i) 
	{
        sum += sampleAO(ssCoord, viewPos, viewNormal, ssDiskRadius, i, randomPatternRotationAngle);
    }

    float AO = max(0.0, 1.0 - sum * u_intensityDivRPow6 * (5.0 / NUM_SAMPLES));


	if (abs(dFdx(viewPos.z)) < 0.02) {
        AO -= dFdx(AO) * ((ssCoord.x & 1) - 0.5);
    }
    if (abs(dFdy(viewPos.z)) < 0.02) {
        AO -= dFdy(AO) * ((ssCoord.y & 1) - 0.5);
    }
    	res.r = AO;

	//gl_FragColor = vec4(res, 1);
	// gl_FragColor = vec4(vec3(res.g), 1);
	//gl_FragColor = vec4(vec3(res.r), 1);
	// gl_FragColor = vec4(0.4);

	// float a;
	//gl_FragColor = vec4(getOffsetPosition(ivec2(0), vec2(0), a), 1);

	//gl_FragColor = vec4((viewPos) , 1);

	//gl_FragColor = vec4(viewNormal, 1);
	gl_FragColor = vec4(res, 1);


/*
	vec2 coord = gl_FragCoord.xy / vec2(1280, 720);
	vec4 RT2_1 = texture(u_RT2, coord);
	float linearZ = recoverDepth(RT2_1.zw);
    vec3 posq = getViewSpacePositionFromLinearZ(linearZ, u_farPlane, v_viewRay);
	gl_FragColor = vec4(posq, 1);

	*/


} 