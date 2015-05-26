#include "Shaders/gbufferUtil.glsl"

/** Increase to make depth edges crisper. Decrease to reduce flicker. */
#define EDGE_SHARPNESS     (1.0)
#define SCALE               (2)
/** Filter radius in pixels. This will be multiplied by SCALE. */
#define R                   (4)

const float gaussian[R + 1] = 
    float[](0.153170, 0.144893, 0.122649, 0.092902, 0.062970);  // stddev = 2.0

uniform sampler2D u_sao;
/** (1, 0) or (0, 1)*/
uniform ivec2 u_axis;

float getDepth(vec4 saoSample)
{
	vec2 packedDepth = saoSample.gb;
    float depth = recoverFloat2x8(packedDepth);
	return depth;

//	vec2 packedDepth = saoSample.gb;
//	float linearZ = recoverDepth(packedDepth);

	//return linearZ;
}

void main() 
{
    ivec2 ssCoord = ivec2(gl_FragCoord.xy);
    vec4 saoSample = texelFetch(u_sao, ssCoord, 0);
	vec2 packedDepth = saoSample.gb;
	float depth = getDepth(saoSample);
//	float depth = saoSample.g;
    float sum = saoSample.r;

	if (depth == 1.0) discard;

    // Base weight for depth falloff.  Increase this for more blurriness,
    // decrease it for better edge discrimination
    float BASE = gaussian[0];
    float totalWeight = BASE;
    sum *= totalWeight;
   
    for (int r = -R; r <= R; ++r) 
	{
        // We already handled the zero case above.  This loop should be unrolled and the static branch optimized out,
        // so the IF statement has no runtime cost
        if (r != 0) 
		{
            saoSample = texelFetch(u_sao, ssCoord + u_axis * (r * SCALE), 0);
            float tapDepth = getDepth(saoSample);
			//float tapDepth = saoSample.g;
            float ao = saoSample.r;
            
            // spatial domain: offset gaussian tap
            float weight = 0.3 + gaussian[abs(r)];
            
            // range domain (the "bilateral" weight). As depth difference increases, decrease weight.
            weight *= max(0.0, 1.0
                - (EDGE_SHARPNESS * 2000.0) * abs(tapDepth - depth));

            sum += ao * weight;
            totalWeight += weight;
        }
    }
 
    const float epsilon = 0.0001;
    float result = sum / (totalWeight + epsilon);	

	// gl_FragColor = vec4(result, packedDepth, 1);
	gl_FragColor = vec4(result, depth, 0, 1);
}
