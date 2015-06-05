uniform sampler2D u_currFrame;
uniform sampler2D u_lastFrame;
varying vec2 v_uvcoord;

void main()
{
	vec3 currentValue = texture(u_currFrame, v_uvcoord).rgb;
    vec3 lastValue = texture(u_lastFrame, v_uvcoord).rgb;
    gl_FragColor = mix(currentValue, lastValue, 0.5f);
    //gl_FragColor = vec4(currentValue, 1);
	// gl_FragColor = vec4(lastValue, 1);
}