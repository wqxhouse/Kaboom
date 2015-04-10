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
 