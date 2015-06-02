#version 120
const float c_inflate = 0.3;
varying vec3 v_vsNormal;
varying vec3 v_position;
 
void main()
{
	vec4 vsPos = gl_ModelViewMatrix * gl_Vertex;
	vec4 Po = vec4(vsPos.xyz, 1);

	vec3 vsNormal = gl_NormalMatrix * gl_Normal;

	v_vsNormal = normalize(vsNormal);

	Po += (c_inflate * vec4(vsNormal, 0));
	v_position = Po.xyz;

	gl_Position = gl_ProjectionMatrix * Po;
}