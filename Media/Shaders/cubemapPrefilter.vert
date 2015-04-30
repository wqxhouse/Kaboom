#version 120

varying	vec3 v_normalWorldSpace;
void main( void )
{
	v_normalWorldSpace = normalize(gl_Normal);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
