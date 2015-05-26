//struct Light 
//{
//    vec3 position;  
//	float radius;
//    vec3 color;	    
//    vec3 dirFromLight;  
//	float padding;
//	int shadowMapIndex[6]; // std140 made this 16 * 6 bytes instead of 4 * 6 bytes, which sucks
//};

//vec3 computeSpecular(vec3 specularColor, float roughness, float NxL, float LxH, float NxV, float NxH) 
//{
//    float specularD = DistributionBRDF(roughness, NxH);
//    float specularG = GeometricBRDF(roughness, NxV, NxL);
//    vec3  specularF = FresnelBRDF(specularColor, LxH);
//    return genericMicrofacetBRDF(specularF, specularG, specularD, NxL, NxV);
//}

//vec3 computeSpecularV2(vec3 specularColor, float roughness, float NoL, float NoV, float NoH, float VoH)
//{
//	float D = DistributionBRDF(roughness, NoH); // TODO: energy conservation
//	float Vis = Vis_SmithJointApprox(roughness, NoV, NoL);
//	vec3 F = F_Schlick(specularColor, VoH);

//	return D * Vis * F; 
//}

//vec3 computeLightModel(Light light, Material material, 
//        vec3 l, vec3 v, vec3 n, vec3 h, 
//        float attenuation, float shadowFactor) 
//{
//        // if (shadowFactor < 0.001) return vec3(0.0);

//    vec3 lightingResult = vec3(0);
//    //vec3 specularColor = mix(vec3(1), material.baseColor, material.metallic) * material.specular;
//    //vec3 diffuseColor = mix(material.baseColor, vec3(0), material.metallic);
//	vec3 specularColor = material.specularColor;
//	vec3 diffuseColor = material.diffuseColor;

//    float roughness = clamp(material.roughness, 0.001, 1.0);

//    float NoL = clamp(dot(n, l), 0.0, 1.0);
//    // float LoH = clamp(dot(l, h), 0.0, 1.0);
//    float NoV = abs(dot(n, v)) + 1e-5;
//    float NoH = clamp(dot(n, h), 0.0, 1.0);
//	float VoH = clamp(dot(v, h), 0.0, 1.0);

//    // Evaluate specular
//    // vec3 specularContribution = computeSpecular(specularColor, roughness, NoL, LoH, NoV, NoH) * NoL;
//	vec3 specularContribution = computeSpecularV2(specularColor, roughness, NoL, NoV, NoH, VoH);

//    // Energy conservation
//    // specularContribution *= pow(1.0 + roughness*0.5, 2.0);
//    lightingResult += specularContribution;

//    // Evaluate diffuse
//	// Lambertian, fast; but currently have seams between two point lights ocurring on the edge of attenuation radius
//    vec3 diffuseContribution = diffuseColor / M_PI;
//	// This is better, but I guess too costly
//	//vec3 diffuseContribution = DiffuseBRDF(diffuseColor, roughness, NoV, NoL, VoH);
//    lightingResult += diffuseContribution;

//	vec3 commonTerm = NoL * light.color * shadowFactor * attenuation; // TODO: implement shadow later 
//	lightingResult *= commonTerm;

//	// debug
//	// lightingResult = pow(lightingResult, vec3(1/2.2));
//    return lightingResult;
//}

//float computePointLightAttenuation(Light light, float distanceToLight) 
//{
//    // Hard transition
//    // return distanceToLight < light.radius ? 1.0 : 0.0;

//    // Soft transition
//    float attenuation = pow(1.0 + (distanceToLight / light.radius) , -2.0) * 1.2;
//    // Cut light transition starting at 80% because the curve is exponential and never gets really 0
//    float cutoff = light.radius * 0.8;
//    attenuation *= 1.0 - smoothstep(0.0, 1.0, ( (distanceToLight / cutoff) - 1.0) * 4.0 );
//    attenuation = max(0.0, attenuation);
//    return attenuation;
//}

//vec3 applyDirectionalLight(Light light, Material material) 
//{
//    float attenuation = 1.0;
//    // no att for dir light
	
//    vec3 l = -light.dirFromLight;
//    //vec3 v = normalize(eyePosition - material.position);
//    vec3 v = normalize(-material.position);
//    vec3 n = normalize(material.normal);
//    vec3 h = normalize(l + v);
//    return computeLightModel(light, material, l, v, n, h, attenuation, 1.0);
//}

//vec3 applyPointLight(Light light, Material material) 
//{
//    float distanceToLight = distance(material.position, light.position);
//    float attenuation = computePointLightAttenuation(light, distanceToLight);
//    vec3 l = normalize(light.position - material.position);
//    // vec3 v = normalize(eyePosition - material.position);
//    vec3 v = normalize(-material.position);
//    vec3 n = normalize(material.normal);
//    vec3 h = normalize(l + v);
//    return computeLightModel(light, material, l ,v, n, h, attenuation, 1.0);
//	//return vec3(attenuation);
//}

////vec3 applyShadowPointLight(Light light, Material material, 
////	in sampler2D u_shadowAtlas, in samplerCube u_shadowCube, 
////	in mat4 viewInvMat, in struct ShadowDepthMap depthMap[MAX_SHADOW_MAPS])
////vec3 applyShadowPointLight(Light light, Material material, ShadowDepthMap shadowInfo, sampler2D u_shadowAtlas)
////{
////	float distanceToLight = distance(material.position, light.position);   
////    float attenuation = computePointLightAttenuation(light, distanceToLight);

////    vec3 l = normalize(light.position - material.position);
////    vec3 v = normalize(-material.position);
////    vec3 n = normalize(material.normal);
////    vec3 h = normalize(l + v);

////	vec3 l_ws = u_viewInvMat * vec4(l, 0).xyz;
//	//int face = int(textureLod(u_shadowCube, l_ws, 0).r * 5.0);
// //   int shadowMapIndex = light.shadowMapIndex[face];
// //   ShadowDepthMap shadowInfo = u_shadowDepthMap[shadowMapIndex]; 
   
////    float shadowFactor = computePointLightShadow(u_shadowAtlas, shadowInfo, material, n, l, 0.2, 0.001, 0.0015);

////    return computeLightModel(light, material, l, v, n, h, attenuation, shadowFactor);
////	// return vec3(1);
////}

//// Modified version of Unreal 4's
//// Need more precision for roughness below 0.5
//// But still keep a the reflection to unnoticable level 
//// when roughness reaching 1.0
//const int roughest_mip = 1; 
//const float roughness_mip_scale = 1.2;
//const int correction = 1; // for gaining precision close to r = 0
//float getLodFromRoughness(float roughness, int maxLodLevel)
//{
//	float LevelFrom0x0 = roughest_mip - roughness_mip_scale * log2(roughness);
//	// return maxLodLevel - correction - LevelFrom0x0;
//	return maxLodLevel - LevelFrom0x0;

//	// 128 -> 8 mips
//	// from 1x1 = 1 - 1.2 * log2(1.0) = 1;
//	// from 1x1 = 1 - 1.2 * log2(0.5) = 1 - 1.2 * (-1) = 1 + 1.2 = 2.2;
//	// from 1x1 = 1 - 1.2 * log2(0.1) = 1 - 1.2 * (-3.32) ~= 1 + 4 = 5;

//	// 1.0:   lod = 7 - 1 - 1 = 5;
//	// 0.5:   lod = 7 - 1 - 2.2 = 4.8 ~= 4;
//	// 0.1:   lod = 7 - 1 - 5 ~= 1;

//	// To get lod 1, roughness = 0.099 ~= 0.1;
//	// To get lod 0, roughness = 0.05;
//	// Thus from 0 -> 1, 5 steps in the editor (0.1 step)
//	// Actual lowest mipmap 4x4.
//}

//float getRoughnessFromLod(float lod, int maxLodLevel)
//{
//	// float levelFrom0x0 = maxLodLevel - correction - lod;
//	float levelFrom0x0 = maxLodLevel - lod;
//	return exp2( (roughest_mip - levelFrom0x0) / roughness_mip_scale );
//}

//// https://www.unrealengine.com/blog/physically-based-shading-on-mobile
//vec3 EnvBRDFApprox( vec3 SpecularColor, float Roughness, float NoV )
//{
//	const vec4 c0 = vec4( -1, -0.0275, -0.572, 0.022 );
//	const vec4 c1 = vec4( 1, 0.0425, 1.04, -0.04 );
//	vec4 r = Roughness * c0 + c1;
//	float a004 = min( r.x * r.x, exp2( -9.28 * NoV ) ) * r.x + r.y;
//	vec2 AB = vec2( -1.04, 1.04 ) * a004 + r.zw;
//	return SpecularColor * AB.x + AB.y;
//}

//vec3 EnvBRDF( in sampler2D lutTex, vec3 SpecularColor, float Roughness, float wsNoV )
//{
//	// Importance sampled preintegrated G * F
//	vec2 lut = texture( lutTex, vec2( wsNoV, Roughness ) ).rg;
//	vec3 GF = SpecularColor * lut.x + clamp( 50.0 * SpecularColor.g, 0.0, 1.0 ) * lut.y;
//	return GF;
//}

//vec3 calcEnvContribution(Material material, 
//	in samplerCube diffuseSampler, 
//	in samplerCube specularSampler, 
//	in sampler2D lutSampler,
//	mat4 viewInvMat, int maxLodLevel)
//{
//    float roughness = clamp(material.roughness, 0.005, 1.0);
//	//float roughness4 = pow(roughness, 4);

//	//// get the approximate reflectance
//	//vec3 reflectance = EnvBRDFApprox( material.specularColor, roughness4, wsNdotV );
//	float lod = getLodFromRoughness(material.roughness, maxLodLevel);

//	vec3 v = normalize(-material.position);
//	vec3 n = normalize(material.normal);
//	vec3 v_ws = (viewInvMat * vec4(v, 0)).xyz;
//	vec3 n_ws = (viewInvMat * vec4(n, 0)).xyz;

//	vec3 lookup_ws = -reflect(v_ws, n_ws);
//	float NoV_ws = clamp(dot(n_ws, v_ws), 0.0, 1.0);

//	vec3 diffuseSample = textureLod(diffuseSampler, n_ws, 0).rgb; 
//	vec3 prefilteredSpecular = textureLod(specularSampler, lookup_ws, lod).rgb;

//	vec3 reflectance =  EnvBRDF(lutSampler, material.specularColor, roughness, NoV_ws);

//	// complete the specularIBL
//	vec3 specularIBL = prefilteredSpecular * reflectance;
		
//	// still have to figure out how to do env. irradiance
//	vec3 diffuseIBL = diffuseSample * material.diffuseColor / M_PI;
//	return diffuseIBL + specularIBL;
//	// return diffuseSample;
//}
