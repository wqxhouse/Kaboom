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

float getLinearZTightFromZ(float z) 
{
    float z_n = z * 2.0 - 1.0;
    float z_e = ndcC / (ndcA - z_n * ndcD);
    return z_e;
}

bool sphereInFrustum(Frustum frustum, vec4 pos, float radius) 
{
    bvec4 result;
    result.x = -radius <= dot(frustum.left, pos);
    result.y = -radius <= dot(frustum.right, pos);
    result.z = -radius <= dot(frustum.top, pos);
    result.w = -radius <= dot(frustum.bottom, pos);
    return all(result);
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

bool isPointLightInFrustum(Light light, Frustum frustum) 
{
    vec4 projectedPos = frustum.viewMat * vec4(light.position, 1);
	//return true;

    // Top/Left/Bottom/Right frustum check
    if (sphereInFrustum(frustum, projectedPos, light.radius)) 
	{
        // Project to screen to get actual depth value
        vec4 projectedScreen = frustum.mvpMat * vec4(light.position, 1);
        projectedScreen.xyz = (projectedScreen.xyz / projectedScreen.w);

        // Fetch linear z
        // We have to use linear space because the radius of the light
        // is also linear, and otherwise we couldn't compare the values
        // properly.
        float linearProjZ = getLinearZTightFromZ(projectedScreen.z) * 2.0 - 1.0;

        // Check if the light is < max-depth
        if ( linearProjZ - sqrt_of_2*light.radius < frustum.far ) 
		{
            // Check if the light is > min-depth
            // Maybe this check can be ommitted as it won't happen very often
            if (linearProjZ + sqrt_of_2*light.radius > frustum.near) 
			{
                return true;
            }
        }
    }
    return false;
}