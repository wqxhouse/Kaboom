struct Material 
{
    // vec3 baseColor;
    float roughness;
    float metallic;
    // float specular;
    // float translucency;
    vec3 position;
    vec3 normal;
	vec3 irradiance;

	vec3 specularColor;
	vec3 diffuseColor;
	float sunShadowMask;
};

Material getMaterialFromGBuffer(vec4 RT0, vec4 RT1, vec4 RT2, vec4 RT3, float farPlane, vec3 viewRay) 
{
    Material m;

	float linearZ = recoverDepth(RT2.zw);
    m.position = getViewSpacePositionFromLinearZ(linearZ, farPlane, viewRay);
    m.normal = decodeNormal(RT2.xy);
	
    m.roughness = max(0.001, RT1.x);
    m.metallic = RT1.z;
    // m.translucency = RT0.w;
	m.sunShadowMask = RT0.w;

    vec3 baseColor = RT0.rgb;
    float specular = RT1.y;

	m.irradiance = RT3.rgb;

	// According to Unreal 4
	m.diffuseColor = baseColor - baseColor * m.metallic; 
	m.specularColor = mix(0.08 * vec3(specular), baseColor, m.metallic);

    return m;
}