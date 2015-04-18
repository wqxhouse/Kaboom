varying vec2 v_uvcoord;
varying vec3 v_viewRay;
uniform mat4 u_projInvMat;

void main()
{
    v_uvcoord = gl_MultiTexCoord0.st;
    gl_Position = ftransform();

    float viewX = gl_Position.x;
    float viewY = gl_Position.y;
    float viewZ = 1;

	// TODO: compute the inverse matrix on CPU, or find better way to 
	// reconstruct position from linear depth

    vec4 ndc_corner = u_projInvMat * vec4(viewX, viewY, viewZ, 1);

    ndc_corner /= ndc_corner.w;
    ndc_corner /= ndc_corner.z;
    v_viewRay = ndc_corner.xyz;
}
