uniform sampler2D u_tex1;
uniform sampler2D u_tex2;

uniform float u_farPlane;

varying vec2 v_uvcoord;

varying vec3 v_viewRay;
varying vec3 v_viewPos;

#include "Shaders/gbufferUtil.glsl"

vec3 reconstructV2(float z)
{
	 //vec3 viewRay = vec3(v_viewPos.xy * (-u_farPlane / v_viewPos.z), -u_farPlane);
	  vec3 viewRay = vec3(v_viewPos.xy * (u_farPlane / v_viewPos.z), -u_farPlane);

	 // vec3 pos = vec3(v_viewPos.xy, -u_farPlane * z);
	 
     vec3 positionVS = z * viewRay;
	 return vec3(1, 0, 1);
}

void main()
{
	vec4 sample1 = texture(u_tex1, v_uvcoord);
	vec4 sample2 = texture(u_tex2, v_uvcoord);

	// reconstruct from z
	float z = sample1.a;
    //vec3 pos = getViewSpacePositionFromLinearZ(z, u_farPlane, v_viewRay);
	vec3 pos = reconstructV2(z);
	vec4 posW = vec4(pos, 1);

	vec4 diff = (sample2 - posW);
	//gl_FragColor = diff * 100000;
	// gl_FragColor = sample1;
	//gl_FragColor = vec4(v_viewPos, 1);
	gl_FragColor = vec4(pos, 1);
}