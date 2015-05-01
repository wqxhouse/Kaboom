// Interesting page on Hammersley Points
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html#
//vec2 Hammersley(const in int index, const in int numSamples ){
//	int reversedIndex = index;
//	reversedIndex = (reversedIndex << 16) | (reversedIndex >> 16);
//	reversedIndex = ((reversedIndex & 0x00ff00ff) << 8) | ((reversedIndex & 0xff00ff00) >> 8);
//	reversedIndex = ((reversedIndex & 0x0f0f0f0f) << 4) | ((reversedIndex & 0xf0f0f0f0) >> 4);
//	reversedIndex = ((reversedIndex & 0x33333333) << 2) | ((reversedIndex & 0xcccccccc) >> 2);
//	reversedIndex = ((reversedIndex & 0x55555555) << 1) | ((reversedIndex & 0xaaaaaaaa) >> 1);
	
//	return vec2(fract(float(index) / numSamples), float(reversedIndex) * 2.3283064365386963e-10);
//}
vec2 Hammersley(uint i, uint N) 
{
    float ri = bitfieldReverse(i) * 2.3283064365386963e-10f;
    return vec2(float(i) / float(N), ri);
}

// straight from Epic paper for Siggraph 2013 Shading course
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_slides.pdf

vec3 ImportanceSampleGGX( vec2 Xi, float Roughness4, vec3 N ) 
{
	float Phi = 2.0f * M_PI * Xi.x;
	float CosTheta = sqrt( (1.0f - Xi.y) / ( 1.0f + (Roughness4 - 1.0f) * Xi.y ) );
	float SinTheta = sqrt( 1.0f - CosTheta * CosTheta );
	
	vec3 H;
	H.x = SinTheta * cos( Phi );
	H.y = SinTheta * sin( Phi );
	H.z = CosTheta;
	
	vec3 UpVector = abs( N.z ) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
	vec3 TangentX = normalize( cross( UpVector, N ) );
	vec3 TangentY = cross( N, TangentX );
	
	// Tangent to world space
	return TangentX * H.x + TangentY * H.y + N * H.z;
}

vec3 ImportanceSampleDiffuse(vec2 Xi, vec3 N)
{
    float CosTheta = 1.0-Xi.y;
    float SinTheta = sqrt(1.0-CosTheta*CosTheta);
    float Phi = 2*M_PI*Xi.x;

    vec3 H;
    H.x = SinTheta * cos( Phi );
    H.y = SinTheta * sin( Phi );
    H.z = CosTheta;

    vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
    vec3 TangentX = normalize( cross( UpVector, N ) );
    vec3 TangentY = cross( N, TangentX );

    return TangentX * H.x + TangentY * H.y + N * H.z;
}
