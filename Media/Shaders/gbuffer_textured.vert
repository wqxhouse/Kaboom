#version 120
varying vec3 v_ws_normal;

varying vec4 v_position;
varying float v_depth;
varying vec2 v_geomTexCoord;
uniform float u_farPlane;

uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ViewMatrix;

mat4 getModelToWorldMatrix()
{
    return osg_ViewMatrixInverse * gl_ModelViewMatrix;
}

void main()
{
    v_position = gl_ModelViewMatrix * gl_Vertex;
	v_geomTexCoord = gl_MultiTexCoord0.st;

	mat4 modelToWorldMat = getModelToWorldMatrix();
    v_ws_normal = normalize(modelToWorldMat * vec4(gl_Normal.xyz, 0)).xyz;

    v_depth = -v_position.z / u_farPlane;
    gl_Position = ftransform();
}
