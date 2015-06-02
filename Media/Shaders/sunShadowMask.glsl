#include "Shaders/constant.glsl"
#include "Shaders/shadowUtil.glsl"

uniform mat4 u_vwvp_sun;
uniform vec2 u_atlas_uvcoord_sun;
uniform float u_tex_scale_sun;
uniform sampler2DShadow u_shadowAtlas;
uniform vec3 u_dirFromSun_vs;

uniform float u_slopeScaledBias;
uniform float u_normalScaledBias;
uniform float u_baseBias;

float getSunShadowMask(vec3 vs_position, vec3 vs_normalized_normal)
{
	vec3 n = vs_normalized_normal;
	vec3 l = -u_dirFromSun_vs;

	return computeDirectionalLightShadowMask(u_shadowAtlas, vs_position, n, l, u_vwvp_sun, u_atlas_uvcoord_sun, 
										     u_tex_scale_sun, u_slopeScaledBias, u_normalScaledBias, u_baseBias);
}
