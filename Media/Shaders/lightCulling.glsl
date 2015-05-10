struct Frustum 
{
    vec4 left;
    vec4 right;
    vec4 top;
    vec4 bottom;
    float near;
    float far;

    mat4 viewMat;
    mat4 mvpMat;
};

struct FrustumDebug
{
	vec4 frustumPlanes[6];	
};

float ndcNear = u_nearPlane;
float ndcFar = u_farPlane;
float ndcA = ndcNear + ndcFar;
float ndcB = ndcNear - ndcFar;
float ndcC = 2.0 * ndcNear * ndcFar;
float ndcD = ndcFar - ndcNear;

float getLinearZTightFromLinearZ(float linz)
{
	return (linz * ndcFar - ndcNear) / ndcD;
}

float getLinearZFromLinearZTight(float linz_tight)
{
	return (linz_tight * ndcD + ndcNear) / ndcFar;
}

float getViewZFromLinearZ(float linz)
{
	return -linz * ndcFar;
}

float getViewZFromTightLinearZ(float tightLinz)
{
	return -(tightLinz * ndcD + ndcNear);
}

float getViewZFromZ(float z) 
{
    float z_n = z * 2.0 - 1.0;
    float z_e = ndcC / (ndcA - z_n * ndcD);
    return -z_e;
}

bool sphereInFrustumDebug(FrustumDebug frustum, vec4 pos, float radius)
{
	bool inFrustum = true;
	for (uint i = 0; i < 6; ++i) 
	{
		float d = dot(frustum.frustumPlanes[i], pos);
		inFrustum = inFrustum && (d >= -radius);
    }

	return inFrustum;
}

bool isPointLightInFrustumDebug(Light light, FrustumDebug fd)
{
	return sphereInFrustumDebug(fd, vec4(light.position, 1.0), light.radius);
}
