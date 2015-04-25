// Zonglin (Robin) Wu, 3/29/2015
// Utility for GBuffer packing 

vec2 encodeNormal(vec3 n)
{
    float p = sqrt(n.z * 8 + 8);
    vec2 enc_spheremap = n.xy / p + 0.5;
    return enc_spheremap;
}

vec3 decodeNormal(vec2 encNormal)
{
    vec2 fenc = encNormal.xy * 4 - 2;
    float f = dot(fenc, fenc);
    float g = sqrt(1 - f / 4);
    vec3 normal;
    normal.xy = fenc * g;
    normal.z = 1 - f / 2; // OpenGL z neg
	return normal;
}

vec2 splitDepth2x16(float depth)
{
    const vec2 bitSh = vec2(65536.0, 1.0);
    const vec2 bitMsk = vec2(0.0, 1.0 / 65536.0);
    vec2 result = fract(depth * bitSh);
    result -= result * bitMsk;
    return result;
}

float recoverDepth(vec2 split_depth)
{
	const vec2 unshift = vec2(1.0 / 65536.0, 1.0);
	return dot(split_depth, unshift);  
}
 
vec3 getViewSpacePositionFromLinearZ(float linz, float farPlane, vec3 viewRay)
{
	return viewRay * (-linz * farPlane);	
}

void reconstructTanBin(vec3 position, vec3 normal, vec2 texcoord, out vec3 tangent, out vec3 binormal) 
{
    //vec3 Q1 = dFdx(vOutput.positionWorld);
    //vec3 Q2 = dFdy(vOutput.positionWorld);
    //vec2 st1 = dFdx(vOutput.texcoord);
    //vec2 st2 = dFdy(vOutput.texcoord);
 
    //tangent = normalize(Q1*st2.t - Q2*st1.t);
    //binormal = normalize(cross(tangent, vOutput.normalWorld));

	vec3 Q1 = dFdx(position);
    vec3 Q2 = dFdy(position);
    vec2 st1 = dFdx(texcoord);
    vec2 st2 = dFdy(texcoord);
 
    tangent = normalize(Q1*st2.t - Q2*st1.t);
    binormal = normalize(cross(tangent, normal));
}