#version 130
#include "Shaders/gbufferUtil.glsl"

varying vec4 v_position;
varying float v_depth;
varying vec2 v_geomTexCoord;

// varying vec3 v_ws_vertex;
varying vec3 v_ws_normal;

uniform sampler2D u_albedoTex;
uniform sampler2D u_roughnessTex;
uniform sampler2D u_metallicTex;
uniform sampler2D u_normalMapTex;
// uniform sampler2D u_translucentTex;

uniform vec3 u_albedo;
uniform float u_roughness;
uniform float u_metallic;
uniform float u_specular;

uniform float u_albedoTexLerp;
uniform float u_roughnessTexLerp;
uniform float u_metallicTexLerp;
uniform float u_normalMapLerp;

uniform mat4 osg_ViewMatrix;

void main()
{
	vec3 normalMap = texture(u_normalMapTex, v_geomTexCoord).xyz;
	normalMap = normalMap * 2.0 - 1.0;

	vec3 ws_normal = normalize(v_ws_normal);
	// TODO: benchmark which method is faster
	//vec3 ws_vertex = v_ws_vertex;
	//vec3 ws_tangent;
	//vec3 ws_binormal;
	// reconstructTanBin(ws_vertex, ws_normal, v_geomTexCoord, ws_tangent, ws_binormal); 
	//vec3 mixedNormal = normalize(
	//	ws_tangent * normalMap.x + 
	//	ws_binormal * normalMap.y + 
	//	ws_normal * normalMap.z
	//);

	vec3 normalLerp = mix(vec3(0.0, 0.0, 1.0), normalMap, u_normalMapLerp);
	vec3 mixedNormal = blendNormals(ws_normal, normalLerp);
	vec3 view_normal = normalize(osg_ViewMatrix * vec4(mixedNormal, 0)).xyz;

	vec3 albedoTex = texture(u_albedoTex, v_geomTexCoord).rgb;
	float roughnessTex = texture(u_roughnessTex, v_geomTexCoord).r;
	float metallicTex = texture(u_metallicTex, v_geomTexCoord).r;

	vec3 albedo = mix(u_albedo, albedoTex, u_albedoTexLerp);
	float roughness = mix(u_roughness, roughnessTex, u_roughnessTexLerp);
	float metallic = mix(u_metallic, metallicTex, u_metallicTexLerp);

	// TODO: make transclucent tex later, as deferred shading is hard to support transparent object
	gl_FragData[0] = vec4(albedo, 1.0); // albedo + translucent
	gl_FragData[1] = vec4(roughness, u_specular, metallic, 0.0); // material buffer + unshaded bit(for skybox)
	gl_FragData[2] = vec4(encodeNormal(view_normal), splitDepth2x16(v_depth)); // encoded normal + split linDepth
}