#version 120

//varying vec3 v_normal;
//varying vec3 v_tangent;
//varying vec3 v_binormal;
varying vec3 v_ws_normal;
//varying vec3 v_ws_vertex;

varying vec4 v_position;
varying float v_depth;
varying vec2 v_geomTexCoord;

varying mat3 v_normalMat;

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
	//v_normal = gl_NormalMatrix * gl_Normal;

	// construct tangent space for normal mapping
	// TODO: make sure need normalize here - extra operation ? 
	mat4 modelToWorldMat = getModelToWorldMatrix();
	// v_ws_vertex = vec3(modelToWorldMat * gl_Vertex);
    v_ws_normal = normalize(modelToWorldMat * vec4(gl_Normal.xyz, 0)).xyz;
	//vec3 ws_tangent;
	//vec3 ws_binormal;
	// reconstructTanBin(ws_vertex, ws_normal, v_geomTexCoord, ws_tangent, ws_binormal); 

	// v_normal = normalize(gl_NormalMatrix * gl_Normal);
	//v_tangent = normalize(osg_ViewMatrix * ws_tangent);
	//v_binormal = normalize(osg_ViewMatrix * ws_binormal);

    v_depth = -v_position.z / u_farPlane;
    gl_Position = ftransform();
}
