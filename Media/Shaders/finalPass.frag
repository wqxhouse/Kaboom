#define MIN_AMBIENT 0.1 // I really don't want to do this, but just in case

uniform sampler2D u_saoTex;
uniform sampler2D u_shadingTex;

varying vec2 v_uvcoord;

const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;
const float W = 11.2;

vec3 Uncharted2Tonemap(vec3 x)
{
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

// TODO: tweak effectcompositor to make this buffer the same as
// screen size instaad of render buffer size
void main()
{
	ivec2 ssCoord = ivec2(gl_FragCoord.xy);

	float ao = texelFetch(u_saoTex, ssCoord, 0).r;
	ao = (ao + MIN_AMBIENT) / (1.0 + MIN_AMBIENT);

	vec3 shading = texelFetch(u_shadingTex, ssCoord, 0).rgb;

	// consider if ao should go into hdr tonemap or not
	// ao = pow(ao, 1.0 / 2.2);

	vec3 result = ao * shading;

	// tone mapping
	float ExposureBias = 1.3;
    vec3 curr = Uncharted2Tonemap(ExposureBias * result);
    
    vec3 whiteScale = 1.0 / Uncharted2Tonemap(vec3(W));
    result = curr * whiteScale;

	// gamma correction
	result = pow(result, vec3(1.0 / 2.2));

	// vignette
	result *= 1.0 - smoothstep(0, 1, (length( (v_uvcoord - vec2(0.5, 0.5)) * vec2(1.2, 1.0)  ) - 0.22) ) * 1.0;


	gl_FragColor = vec4(result, 1);
	//gl_FragColor = vec4(vec3(ao), 1);
	//gl_FragColor = vec4(shading, 1);
}
