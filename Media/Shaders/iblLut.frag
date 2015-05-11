#include "Shaders/constant.glsl"
#include "Shaders/lightPBRModel.glsl"
#include "Shaders/importanceSampling.glsl"

#pragma optionNV (unroll none) // hopefully shorten the shader time

vec2 integrate(float roughness, float NoV)
{
    vec3 N = vec3(0.0f, 0.0f, 1.0f);
    vec3 V; 
	V.x = sqrt(1.0f - NoV * NoV);
	V.y = 0;
	V.z = NoV;

    vec2 result = vec2(0,0);

	const int numsamples = 1024;
    for (uint i = 0; i < numsamples; i++)
    {
        vec2 Xi = Hammersley(i, numsamples);
        vec3 H = ImportanceSampleGGX(Xi, roughness, N);
        vec3 L = 2.0f * dot(V, H) * H - V;

        float NoL = saturate(L.z);
        float NoH = saturate(H.z);
        float VoH = saturate(dot(V, H));

        if (NoL > 0)
        {
			float Vis = Vis_SmithJointApprox( roughness, NoV, NoL );

			float a = roughness * roughness;
			float a2 = a*a;
			float Vis_SmithV = NoL * sqrt( NoV * (NoV - NoV * a2) + a2 );
			float Vis_SmithL = NoV * sqrt( NoL * (NoL - NoL * a2) + a2 );
			float NoL_Vis_PDF = NoL * Vis * (4 * VoH / NoH);

			float Fc = pow( 1 - VoH, 5 );
			result.x += (1 - Fc) * NoL_Vis_PDF;
			result.y += Fc * NoL_Vis_PDF;
        }
    }

    result.x = (result.x / float(numsamples)) ;
    result.y = (result.y / float(numsamples)) ;

    return result;
}

void main()
{
	vec2 coord = gl_FragCoord.xy;

	// texsize 256 * 256
	float roughness = coord.y / 256.0;
    float NoV = coord.x / 256.0;
      
    vec2 result = integrate(roughness, NoV); 
	gl_FragColor = vec4(result.x, result.y, roughness, 1);
}