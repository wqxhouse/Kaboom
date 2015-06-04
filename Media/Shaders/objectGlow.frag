#version 120

varying vec3 v_position;
varying vec3 v_vsNormal;

const float c_glowExp = 0.2;
const vec3 c_glowColor = vec3(0.7, 0.7, 0.2);

void main()
{
	vec3 n = normalize(v_vsNormal);
	vec3 v = -normalize(v_position);
	float edge = 1.0 - dot(n, v);
	edge = pow(edge, c_glowExp);
	vec3 res = edge * c_glowColor;
	gl_FragColor = vec4(res, edge);
}