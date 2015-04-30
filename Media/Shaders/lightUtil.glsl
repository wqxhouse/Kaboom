struct Light 
{
    vec3 position;
    vec3 color;
    vec3 dirFromLight;
    // directional light & spot light
    float radius;
    // point light
};

vec3 computeSpecular(vec3 specularColor, float roughness, float NxL, float LxH, float NxV, float NxH) 
{
    float specularD = DistributionBRDF(roughness, NxH);
    float specularG = GeometricBRDF(roughness, NxV, NxL);
    vec3  specularF = FresnelBRDF(specularColor, LxH);
    return genericMicrofacetBRDF(specularF, specularG, specularD, NxL, NxV);
}

vec3 computeSpecularV2(vec3 specularColor, float roughness, float NoL, float NoV, float NoH, float VoH)
{
	float D = DistributionBRDF(roughness, NoH); // TODO: energy conservation
	float Vis = Vis_SmithJointApprox(roughness, NoV, NoL);
	vec3 F = F_Schlick(specularColor, VoH);

	return D * Vis * F; 
}

//float3 StandardShading( FGBufferData GBuffer, float3 LobeRoughness, float3 LobeEnergy, float3 L, float3 V, half3 N, float2 DiffSpecMask )
//{
//	float3 H = normalize(V + L);
//	float NoL = saturate( dot(N, L) );
//	//float NoV = saturate( dot(N, V) );
//	float NoV = abs( dot(N, V) ) + 1e-5;
//	float NoH = saturate( dot(N, H) );
//	float VoH = saturate( dot(V, H) );
	
//	// Generalized microfacet specular
//	float D = D_GGX( LobeRoughness[1], NoH ) * LobeEnergy[1];
//	float Vis = Vis_SmithJointApprox( LobeRoughness[1], NoV, NoL );
//	float3 F = F_Schlick( GBuffer.SpecularColor, VoH );

//	return Diffuse_Lambert(GBuffer.DiffuseColor) * (LobeEnergy[2] * DiffSpecMask.r) + (D * Vis * DiffSpecMask.g) * F;
//}

//// accumulate diffuse and specular
//			{
//#if 1	// for testing if there is a perf impact
//				// correct screen space subsurface scattering
//				float3 SurfaceLightingDiff = SurfaceShading(ScreenSpaceData.GBuffer, LobeRoughness, LobeEnergy, L, V, N, float2(1, 0));
//				float3 SurfaceLightingSpec = SurfaceShading(ScreenSpaceData.GBuffer, LobeRoughness, LobeEnergy, L, V, N, float2(0, 1));
//				LightAccumulator_Add(LightAccumulator, SurfaceLightingDiff, SurfaceLightingSpec, LightColor * (NoL * SurfaceAttenuation));
//#else
//				// wrong screen space subsurface scattering but potentially faster
//				float3 SurfaceLighting = SurfaceShading(ScreenSpaceData.GBuffer, LobeRoughness, LobeEnergy, L, V, N, float2(1, 1));
//				LightAccumulator_Add(LightAccumulator, SurfaceLighting, 0, LightColor * (NoL * SurfaceAttenuation));
//#endif
//			}



vec3 computeLightModel(Light light, Material material, 
        vec3 l, vec3 v, vec3 n, vec3 h, 
        float attenuation, float shadowFactor) 
{
        // if (shadowFactor < 0.001) return vec3(0.0);

    vec3 lightingResult = vec3(0);
    //vec3 specularColor = mix(vec3(1), material.baseColor, material.metallic) * material.specular;
    //vec3 diffuseColor = mix(material.baseColor, vec3(0), material.metallic);
	vec3 specularColor = material.specularColor;
	vec3 diffuseColor = material.diffuseColor;

    float roughness = clamp(material.roughness, 0.001, 1.0);

    float NoL = clamp(dot(n, l), 0.0, 1.0);
    // float LoH = clamp(dot(l, h), 0.0, 1.0);
    float NoV = abs(dot(n, v)) + 1e-5;
    float NoH = clamp(dot(n, h), 0.0, 1.0);
	float VoH = clamp(dot(v, h), 0.0, 1.0);

    // Evaluate specular
    // vec3 specularContribution = computeSpecular(specularColor, roughness, NoL, LoH, NoV, NoH) * NoL;
	vec3 specularContribution = computeSpecularV2(specularColor, roughness, NoL, NoV, NoH, VoH);

    // Energy conservation
    // specularContribution *= pow(1.0 + roughness*0.5, 2.0);
    lightingResult += specularContribution;

    // Evaluate diffuse
	// Lambertian
    vec3 diffuseContribution = diffuseColor / M_PI;
	// This is better, but I guess too costly
	// vec3 diffuseContribution = DiffuseBRDF(diffuseColor, roughness, NoV, NoL, VoH);
    lightingResult += diffuseContribution;

	vec3 commonTerm = NoL * light.color * shadowFactor * attenuation; // TODO: implement shadow later 
	lightingResult *= commonTerm;

	// debug
	// lightingResult = pow(lightingResult, vec3(1/2.2));
    return lightingResult;
}

float computePointLightAttenuation(Light light, float distanceToLight) 
{
    // Hard transition
    // return distanceToLight < light.radius ? 1.0 : 0.0;

    // Soft transition
    float attenuation = pow(1.0 + (distanceToLight / light.radius) , -2.0) * 1.2;
    // Cut light transition starting at 80% because the curve is exponential and never gets really 0
    float cutoff = light.radius * 0.8;
    attenuation *= 1.0 - smoothstep(0.0, 1.0, ( (distanceToLight / cutoff) - 1.0) * 4.0 );
    attenuation = max(0.0, attenuation);
    return attenuation;
}

vec3 applyDirectionalLight(Light light, Material material) 
{
    float attenuation = 1.0;
    // no att for dir light
	
    vec3  l = -light.dirFromLight;
    //vec3  v = normalize(eyePosition - material.position);
    vec3  v = normalize(-material.position);
    vec3  n = normalize(material.normal);
    vec3  h = normalize(l + v);
    return computeLightModel(light, material, l, v, n, h, attenuation, 1.0);
}

vec3 applyPointLight(Light light, Material material) 
{
    float distanceToLight = distance(material.position, light.position);
    float attenuation = computePointLightAttenuation(light, distanceToLight);
    vec3  l = normalize(light.position - material.position);
    // vec3  v = normalize(eyePosition - material.position);
    vec3  v = normalize(-material.position);
    vec3  n = normalize(material.normal);
    vec3  h = normalize(l + v);
    return computeLightModel(light, material, l ,v, n, h, attenuation, 1.0);
	//return vec3(attenuation);
}

// https://www.unrealengine.com/blog/physically-based-shading-on-mobile
vec3 EnvBRDFApprox( vec3 SpecularColor, float Roughness, float NoV )
{
	const vec4 c0 = vec4( -1, -0.0275, -0.572, 0.022 );
	const vec4 c1 = vec4( 1, 0.0425, 1.04, -0.04 );
	vec4 r = Roughness * c0 + c1;
	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
	vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;
	return SpecularColor * AB.x + AB.y;
}

// http://the-witness.net/news/2012/02/seamless-cube-map-filtering/
vec3 fix_cube_lookup( vec3 v, float cube_size, float lod ) 
{
	float M = max(max(abs(v.x), abs(v.y)), abs(v.z));
	float scale = 1 - exp2(lod) / cube_size;
	if (abs(v.x) != M) v.x *= scale;
	if (abs(v.y) != M) v.y *= scale;
	if (abs(v.z) != M) v.z *= scale;
	return v;
}

vec3 calcEnvContribution(Material material, vec3 diffuseSample, vec3 specularSample, float wsNdotV)
{
    float roughness = clamp(material.roughness, 0.005, 1.0);
	float roughness4 = pow(roughness, 4);

	// get the approximate reflectance
	vec3 reflectance = EnvBRDFApprox( material.specularColor, roughness4, wsNdotV );
		
	// combine the specular IBL and the BRDF
	vec3 specularIBL		= specularSample * reflectance;
		
	// still have to figure out how to do env. irradiance
	vec3 diffuseIBL			= diffuseSample * material.diffuseColor;
		
	// not sure how to combine this with the rest
	return diffuseIBL + specularIBL;
}

float getLodFromRoughness(float roughness, int cubeMaxLod)
{
	return 0.0;
}
