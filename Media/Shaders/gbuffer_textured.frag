#version 130
#include "Shaders/gbufferUtil.glsl"

varying vec4 v_position;
varying float v_depth;
varying vec2 v_geomTexCoord;

varying vec3 v_ws_vertex;
varying vec3 v_ws_normal;
//varying vec3 v_tangent;
//varying vec3 v_binormal;

uniform sampler2D u_albedoTex;
uniform sampler2D u_roughnessTex;
uniform sampler2D u_specularTex;
uniform sampler2D u_metallicTex;
uniform sampler2D u_normalMapTex;
// uniform sampler2D u_translucentTex;

void main()
{
	vec3 albedo = texture(u_albedoTex, v_geomTexCoord).rgb;
	vec3 normalMap = texture(u_normalMapTex, v_geomTexCoord).xyz;
	normalMap = normalMap * 2.0 - 1.0;

	vec3 ws_normal = normalize(v_ws_normal);
	vec3 ws_vertex = v_ws_vertex;
	vec3 ws_tangent;
	vec3 ws_binormal;

	reconstructTanBin(ws_vertex, ws_normal, v_geomTexCoord, ws_tangent, ws_binormal); 

	vec3 mixedNormal = normalize(
		ws_tangent * normalMap.x + 
		ws_binormal * normalMap.y + 
		ws_normal * normalMap.z
	);

	// TODO: check if necessary to normalize
	mixedNormal = normalize(mixedNormal);

	vec3 view_normal = gl_NormalMatrix * mixedNormal;

	float specular = texture(u_specularTex, v_geomTexCoord).r;
	float roughness = texture(u_roughnessTex, v_geomTexCoord).r;
	float metallic = texture(u_metallicTex, v_geomTexCoord).r;

	// TODO: make transclucent tex later, as deferred shading is hard to support transparent object
	gl_FragData[0] = vec4(albedo, 1.0); // albedo + translucent
	gl_FragData[1] = vec4(roughness, specular, metallic, 1.0); // material buffer
	gl_FragData[2] = vec4(encodeNormal(view_normal), splitDepth2x16(v_depth)); // encoded normal + split linDepth
}