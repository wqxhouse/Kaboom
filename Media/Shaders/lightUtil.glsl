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


//float computeMipmapFromRoughness(float roughness) {
//    return max(0.0, fallbackCubemapMipmaps - 11 +  pow(roughness, 0.25) * 11.0);
//}

vec3 computeLightModel(Light light, Material material, 
        vec3 l, vec3 v, vec3 n, vec3 h, 
        float attenuation, float shadowFactor) 
{
    // Performance optimization
        // if (shadowFactor < 0.001) return vec3(0.0);

    vec3 lightingResult = vec3(0);
    vec3 specularColor = mix(vec3(1), material.baseColor, material.metallic) * material.specular;
    vec3 diffuseColor = mix(material.baseColor, vec3(0), material.metallic);
    float roughness = clamp(material.roughness, 0.005, 1.0);
    // pre-computed cross products
    float NxL = clamp(dot(n, l), 0.0, 1.0);
    float LxH = clamp(dot(l, h), 0.0, 1.0);
    float NxV = abs(dot(n, v)) + 1e-5;
    float NxH = clamp(dot(n, h), 0.0, 1.0);
    // Evaluate specular
    vec3 specularContribution = computeSpecular(specularColor, roughness, NxL, LxH, NxV, NxH) * NxL;
    // Energy conservation
        // specularContribution *= pow(1.0 + roughness*0.5, 2.0);
    lightingResult += specularContribution * material.baseColor * light.color;
    // Evaluate diffuse
    vec3 diffuseContribution = diffuseColor / M_PI * NxL * light.color;
    lightingResult += diffuseContribution;
    // Apply shadows
    lightingResult *= shadowFactor;
    // Apply point light attenuation
    lightingResult *= attenuation;
    return lightingResult;
}

vec3 computeEnvironmentBRDF(vec3 specularColor, float roughness, float NxV)
{
    const vec4 a = vec4(-1, -0.0275, -0.572, 0.022);
    const vec4 b = vec4(1, 0.0425, 1.04, -0.04);
    vec4 r = roughness * a + b;
    float c = min( r.x * r.x, exp2( -9.28 * NxV ) ) * r.x + r.y;
    vec2 multiplier = vec2( -1.04, 1.04 ) * c + r.zw;
    return specularColor * multiplier.x + multiplier.y;
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

vec3 applyDirectionalLight(Light light, Material material, vec3 eyePosition) 
{
    float attenuation = 1.0;
    // no att for dir light

    vec3  l = -light.dirFromLight;
    vec3  v = normalize(eyePosition - material.position);
    vec3  n = normalize(material.normal);
    vec3  h = normalize(l + v);
    return computeLightModel(light, material, l, v, n, h, attenuation, 1.0);
}

vec3 applyPointLight(Light light, Material material, vec3 eyePosition) 
{
    float distanceToLight = distance(material.position, light.position);
    float attenuation = computePointLightAttenuation(light, distanceToLight);
    vec3  l = normalize(light.position - material.position);
    vec3  v = normalize(eyePosition - material.position);
    vec3  n = normalize(material.normal);
    vec3  h = normalize(l + v);
    return computeLightModel(light, material, l ,v, n, h, attenuation, 1.0);
	//return vec3(attenuation);
}

