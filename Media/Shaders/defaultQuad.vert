varying vec2 v_uvcoord;

void main()
{
	gl_Position = ftransform();
	v_uvcoord = gl_MultiTexCoord0.st;
}