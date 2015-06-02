#version 400
#include "Shaders/gbufferUtil.glsl"

// OSG 3.2.0 shader readfile bug: commenting #incldue won't get it commented; modified effectcompositor handles that
#include "Shaders/sunShadowMask.glsl"

// varying vec4 v_color;
varying vec3 v_normal;
varying vec4 v_position;
varying float v_depth;

uniform vec3 u_albedo;
uniform float u_roughness;
uniform float u_specular;
uniform float u_metallic;
// uniform float u_translucent;

void main()
{
	vec3 n = normalize(v_normal);
	float sunShadowMask = getSunShadowMask(v_position.xyz, n);

	gl_FragData[0] = vec4(u_albedo, sunShadowMask); // albedo + translucent
	gl_FragData[1] = vec4(u_roughness, u_specular, u_metallic, 0.0); // material buffer + unshaded bit (for skybox)
	gl_FragData[2] = vec4(encodeNormal(n), splitDepth2x16(v_depth)); // encoded normal + split linDepth
	// gl_FragData[2] = vec4(encodeNormal(normalize(v_normal)), 0, 0); // encoded normal + split linDepth

	//gl_FragData[3] = vec4(v_position.xyz, 1);
}