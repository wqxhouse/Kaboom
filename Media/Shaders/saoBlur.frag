#include "Shaders/gbufferUtil.glsl"
#define EDGE_SHARPNESS     (0.8)
#define SCALE               (2)
#define R                   (4)

// TODO: y axis is flickering. figure out

const float gaussian[R + 1] = 
    float[](0.153170, 0.144893, 0.122649, 0.092902, 0.062970);  // stddev = 2.0

uniform sampler2D u_sao;
uniform ivec2 u_axis;

float getDepth(vec4 saoSample)
{
	vec2 packedDepth = saoSample.gb;
    float depth = recoverFloat2x8(packedDepth);
	return depth;

	//vec2 packedDepth = saoSample.gb;
	//float linearZ = recoverDepth(packedDepth);

	//return linearZ;
}

void main() 
{
    ivec2 ssCoord = ivec2(gl_FragCoord.xy);
    vec4 saoSample = texelFetch(u_sao, ssCoord, 0);
	vec2 packedDepth = saoSample.gb;
    float depth = getDepth(saoSample);
    float sum = saoSample.r;

	if (depth == 1.0) discard;

    float BASE = gaussian[0];
    float totalWeight = BASE;
    sum *= totalWeight;
   
    for (int r = -R; r <= R; ++r) 
	{
        if (r != 0) 
		{
            saoSample = texelFetch(u_sao, ssCoord + u_axis * (r * SCALE), 0);
            float tapDepth = getDepth(saoSample);
            float ao = saoSample.r;
            
            float weight = 0.3 + gaussian[abs(r)];
            
            weight *= max(0.0, 1.0
                - (EDGE_SHARPNESS * 2000.0) * abs(tapDepth - depth));

            sum += ao * weight;
            totalWeight += weight;
        }
    }
 
    const float epsilon = 0.0001;
    float result = sum / (totalWeight + epsilon);	

	gl_FragColor = vec4(result, packedDepth, 1);
}
