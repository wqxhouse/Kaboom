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
    //const vec2 bitSh = vec2(65536.0, 1.0);
    //const vec2 bitMsk = vec2(0.0, 1.0 / 65536.0);
    //vec2 result = fract(depth * bitSh);
    //result -= result * bitMsk;
    //return result;

	vec2 p;
    float temp = floor(depth * 65536.0);
    p.x = temp * (1.0 / 65536.0);
    p.y = depth * 65536.0 - temp;
	return p;
}

float recoverDepth(vec2 split_depth)
{
	//const vec2 unshift = vec2(1.0 / 65536.0, 1.0);
	//return dot(split_depth, unshift);  

	return split_depth.x * (65536.0 / 65537.0) + split_depth.y * (1.0 / 65537.0);	
}
 
vec2 splitFloat2x8(float f) 
{
	vec2 res;
    // Round to the nearest 1/256.0
    float temp = floor(f * 256.0);

    // Integer part
    res.x = temp * (1.0 / 256.0);

    // Fractional part
    res.y = f * 256.0 - temp;

	return res;
}

float recoverFloat2x8(vec2 p) 
{
    return p.x * (256.0 / 257.0) + p.y * (1.0 / 257.0);
}

vec3 getViewSpacePositionFromLinearZ(float linz, float farPlane, vec3 viewRay)
{
	return viewRay * (-linz * farPlane);	
}

// http://blog.selfshadow.com/publications/blending-in-detail/
vec3 blendNormals( vec3 baseNormal, vec3 detailsNormal )
{
	vec3 n1 = baseNormal;
	vec3 n2 = detailsNormal;
	mat3 nBasis = mat3(
        vec3(n1.z, n1.y, -n1.x), // +90 degree rotation around y axis
        vec3(n1.x, n1.z, -n1.y), // -90 degree rotation around x axis
        vec3(n1.x, n1.y,  n1.z));
	return normalize(n2.x*nBasis[0] + n2.y*nBasis[1] + n2.z*nBasis[2]);
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
