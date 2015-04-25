struct Material 
{
    vec3 baseColor;
    float roughness;
    float metallic;
    float specular;
    float translucency;
    vec3 position;
    vec3 normal;
};

Material getMaterialFromGBuffer(vec4 RT0, vec4 RT1, vec4 RT2, float farPlane, vec3 viewRay) 
{
    Material m;

	float linearZ = recoverDepth(RT2.zw);
    m.position = getViewSpacePositionFromLinearZ(linearZ, farPlane, viewRay);
    m.normal = decodeNormal(RT2.xy);
	
    m.roughness = max(0.001, RT1.x);
    m.specular = RT1.y;
    m.metallic = RT1.z;
    m.translucency = RT0.w;
    m.baseColor = RT0.rgb;

    return m;
}