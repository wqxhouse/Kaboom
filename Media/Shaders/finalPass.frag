#define MIN_AMBIENT 0.1 // I really don't want to do this, but just in case

uniform sampler2D u_saoTex;
uniform sampler2D u_shadingTex;

// TODO: tweak effectcompositor to make this buffer the same as
// screen size instaad of render buffer size
void main()
{
	ivec2 ssCoord = ivec2(gl_FragCoord.xy);

	float ao = texelFetch(u_saoTex, ssCoord, 0).r;
	ao = (ao + MIN_AMBIENT) / (1.0 + MIN_AMBIENT);

	vec3 shading = texelFetch(u_shadingTex, ssCoord, 0).rgb;

	// consider if ao should go into hdr tonemap or not
	//gl_FragColor = vec4(ao * shading, 1);
	// gl_FragColor = vec4(vec3(ao), 1);

	vec3 depth = texelFetch(u_saoTex, ssCoord, 0).rgb;
	gl_FragColor = vec4(vec3(depth.r), 1);
}
