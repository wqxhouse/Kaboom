varying vec4 offset;
#define SMAA_PIXEL_SIZE vec2(1.0/800, 1.0/600.0)

void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
    offset = gl_TexCoord[0].xyxy + SMAA_PIXEL_SIZE.xyxy * vec4(-1.0, 0.0, 0.0,-1.0);
}
