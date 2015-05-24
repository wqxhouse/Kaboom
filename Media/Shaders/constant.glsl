// Needed for light culling
const float sqrt_of_2 = 1.4142135623;

// #define M_PI 3.14159265
#define saturate(x) clamp(x, 0.0, 1.0)
#define M_PI 3.1415926535897932384626433832795
#define INV_M_PI 0.31830988618

#define LIGHT_TILE_SIZE_X 32
#define LIGHT_TILE_SIZE_Y 32

#define LIGHTING_MIN_MAX_DEPTH_ACCURACY 1
#define MAX_DIRECTIONAL_LIGHTS 1
#define MAX_DIRECTIONAL_SHADOW_LIGHTS 1
//#define MAX_POINT_LIGHTS 512 
//#define MAX_VISIBLE_LIGHTS 256 

#define MAX_POINT_LIGHTS 256 
#define MAX_POINT_SHADOW_LIGHTS 6
#define MAX_VISIBLE_LIGHTS 128 

// #pragma optionNV (unroll all)