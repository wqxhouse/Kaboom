varying vec4 v_color;
varying vec3 v_normal;
varying vec4 v_position;
varying float v_depth;

uniform float u_farPlane;

void main()
{
    // v_color = gl_Color;
    v_position = gl_ModelViewMatrix * gl_Vertex;
    v_normal = gl_NormalMatrix * gl_Normal;
    v_depth = -v_position.z / u_farPlane;
    gl_Position = ftransform();
}
