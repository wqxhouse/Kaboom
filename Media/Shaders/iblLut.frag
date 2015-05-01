#include "Shaders/importanceSampling.glsl"

vec2 sumLut(vec2 current, float G, float V, float F, 
			float VoH, float NoL, float NoH, float NoV)
{
    G = G * V;
    float G_Vis = G * VoH / (NoH * NoV);
    current.x += (1.0 - F) * G_Vis;
    current.y += F * G_Vis;

    return current;
}

vec2 integrate(float roughness, float NoV)
{
    vec3 N = vec3(0.0f, 0.0f, 1.0f);
    vec3 V = vec3(sqrt(1.0f - NoV * NoV), 0.0f, NoV);
    vec2 result = vec2(0,0);

    float Vis = visibilityForLut(roughness, NoV);

    for (uint i = 0; i < NumSamples; i++)
    {
        vec2 Xi = hammersley(i, NumSamples);
        vec3 H = importanceSampleGGX(Xi, roughness, N);
        precise vec3 L = 2.0f * dot(V, H) * H - V;

        float NoL = saturate(L.z);
        float NoH = saturate(H.z);
        float VoH = saturate(dot(V, H));
        float NoV = saturate(dot(N, V));
        if (NoL > 0)
        {
            float G = geometryForLut(roughness, NoL);
            float F = fresnelForLut(VoH);
            result = sumLut(result, G, Vis, F, VoH, NoL, NoH, NoV); 
        }
    }

    result.x = (result.x / float(NumSamples)) ;
    result.y = (result.y / float(NumSamples)) ;

    return result;
}