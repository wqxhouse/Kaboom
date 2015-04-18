/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2011 Robert Osfield
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
*/

#include <osgShadow/ShadowSettings>
#include <sstream>

using namespace osgShadow;

std::string ShadowSettings::ShaderGenerator::generateCasterVertexShader(ShadowSettings* settings) const
{
    static const char vertexShaderSource_VSMCaster[] =
        "varying vec4 v_position;          \n"
        "void main(void)                   \n"
        "{                                 \n"
        "  gl_Position = ftransform();     \n"
        "  v_position = gl_Position;       \n"
        "} \n";

    int shadowMapHints = settings->getShadowMapTechniqueHints();
    if (shadowMapHints & osgShadow::ShadowSettings::VARIANCE)
        return std::string(vertexShaderSource_VSMCaster);
    else
        return std::string();
}

std::string ShadowSettings::ShaderGenerator::generateCasterFragmentShader(ShadowSettings* settings) const
{
    static const char fragmentShaderSource_VSMCaster[] =
        "varying vec4 v_position;                                   \n"    
        "void main(void)                                            \n"
        "{                                                          \n"
        "  float depth = v_position.z / v_position.w;               \n"
		"  depth = depth * 0.5 + 0.5; // [-1,1]) to [0,1];          \n"
		"  float moment1 = depth, moment2 = depth * depth;          \n"
		"  float dx = dFdx(depth), dy = dFdy(depth);                \n"
		"  moment2 += 0.25 * (dx*dx+dy*dy);                         \n"
		"  gl_FragColor = vec4(moment1, moment2, 0.0, 1.0);         \n"
        "} \n";

    int shadowMapHints = settings->getShadowMapTechniqueHints();
    if (shadowMapHints & osgShadow::ShadowSettings::VARIANCE)
        return std::string(fragmentShaderSource_VSMCaster);
    else
        return std::string();
}

std::string ShadowSettings::ShaderGenerator::generateReceiverVertexShader(ShadowSettings* settings) const
{
    unsigned int numShadowMaps = settings->getNumShadowMapsPerLight();
    int shadowMapHints = settings->getShadowMapTechniqueHints();

    std::string shadowSamplerName("sampler2DShadow");
    if (shadowMapHints & osgShadow::ShadowSettings::VARIANCE)
    {
        shadowSamplerName = "sampler2D";
    }

    // Uniforms & predefined functions
    std::stringstream ss;
    ss << "uniform mat4 osg_ViewMatrixInverse;\n"
       << "uniform sampler2D baseTexture;\n"
       << "uniform int baseTextureUnit;\n";
    for (unsigned int i=0; i<numShadowMaps; ++i)
    {
       ss << "uniform int shadowTextureUnit" << i << ";\n";
    }
    ss << "const int numShadowMaps = " << numShadowMaps << ";\n";
    ss << "void setupTexCoord(in int shadowTextureUnit, in vec4 eyePosition) {\n"
       << "    gl_TexCoord[shadowTextureUnit].s = dot(eyePosition, gl_EyePlaneS[shadowTextureUnit]);\n"
	   << "    gl_TexCoord[shadowTextureUnit].t = dot(eyePosition, gl_EyePlaneT[shadowTextureUnit]);\n"
	   << "    gl_TexCoord[shadowTextureUnit].p = dot(eyePosition, gl_EyePlaneR[shadowTextureUnit]);\n"
	   << "    gl_TexCoord[shadowTextureUnit].q = dot(eyePosition, gl_EyePlaneQ[shadowTextureUnit]);\n"
       << "}\n";

    // Main entry
    ss << "void main(void) {\n";
    ss << "  vec4 eyePosition = gl_ModelViewMatrix * gl_Vertex;\n";
    for (unsigned int i=0; i<numShadowMaps; ++i)
    {
        ss << "  setupTexCoord(shadowTextureUnit" << i << ", eyePosition);\n";
    }
    ss << "  gl_TexCoord[baseTextureUnit] = gl_MultiTexCoord0;\n"
       << "  gl_FrontColor = gl_Color;\n"
       << "  gl_Position = ftransform();\n"
       << "}\n";
    return ss.str();
}

std::string ShadowSettings::ShaderGenerator::generateReceiverFragmentShader(ShadowSettings* settings) const
{
    unsigned int numShadowMaps = settings->getNumShadowMapsPerLight();
    const osg::Vec2s& textureSize = settings->getTextureSize();
    int shadowMapHints = settings->getShadowMapTechniqueHints();

    std::string shadowSamplerName("sampler2DShadow");
    std::string readShadowFuncCode("return shadow2DProj(shadowTexture, shadowUV).r;\n");
    if (shadowMapHints & osgShadow::ShadowSettings::VARIANCE)
    {
        shadowSamplerName = "sampler2D";
        readShadowFuncCode =
            "  vec4 shadowCoords = shadowUV / shadowUV.w;                             \n"
            "  vec2 moments = texture2D(shadowTexture, shadowCoords.xy).rg;           \n"
            "  if (moments.x < shadowCoords.z) {                                      \n"
            "     float variance = max(moments.y - (moments.x * moments.x), 0.00002); \n"
            "     float distance = shadowCoords.z - moments.x;                        \n"
            "     return variance / (variance + distance * distance);                 \n"
            "  }                                                                      \n"
            "  return 1.0;                                                            \n";
    }

    // Uniforms & predefined functions
    std::stringstream ss;
    ss << "uniform sampler2D baseTexture;\n"
       << "uniform int baseTextureUnit;\n";
    for (unsigned int i=0; i<numShadowMaps; ++i)
    {
       ss << "uniform " << shadowSamplerName << " shadowTexture" << i << ";\n"
          << "uniform int shadowTextureUnit" << i << ";\n";
       if (shadowMapHints & osgShadow::ShadowSettings::PARALLEL_SPLIT)
           ss << "uniform float shadowSplittedZ" << i << ";\n";
    }
    ss << "const int numShadowMaps = " << numShadowMaps << ";\n";
    ss << "float readShadowValue(in " << shadowSamplerName << " shadowTexture, in vec4 shadowUV) {\n"
       << readShadowFuncCode << "}\n";

    if (shadowMapHints & osgShadow::ShadowSettings::PCF3x3)
    {
        ss << "const float invTexel = " << 1.0f / textureSize.x() << ";\n";
        ss << "float readPCFValue(in " << shadowSamplerName << " shadowTexture, in vec4 shadowUV) {            \n"
           << "  float shadowTerm = readShadowValue(shadowTexture, shadowUV);                                  \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV - vec4(invTexel, 0.0, 0.0, 0.0));       \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV + vec4(invTexel, 0.0, 0.0, 0.0));       \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV - vec4(0.0, invTexel, 0.0, 0.0));       \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV + vec4(0.0, invTexel, 0.0, 0.0));       \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV - vec4(invTexel, invTexel, 0.0, 0.0));  \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV + vec4(invTexel, invTexel, 0.0, 0.0));  \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV - vec4(invTexel,-invTexel, 0.0, 0.0));  \n"
           << "  shadowTerm += readShadowValue(shadowTexture, shadowUV + vec4(invTexel,-invTexel, 0.0, 0.0));  \n"
           << "  return shadowTerm / 9.0;                                                                      \n"
           << "}                                                                                               \n";
    }

    // Main entry
    ss << "void main(void) {\n"
       << "  vec4 colorAmbientEmissive = gl_Color * gl_LightModel.ambient;\n"
       << "  vec4 color = texture2D(baseTexture, gl_TexCoord[baseTextureUnit].xy);\n"
       << "  float shadowTerm = 1.0;\n";
    
    std::string readShadowFuncName("readShadowValue");
    if (shadowMapHints & osgShadow::ShadowSettings::PCF3x3)
        readShadowFuncName = "readPCFValue";
    for (unsigned int i=0; i<numShadowMaps; ++i)
    {
        ss << "  shadowTerm *= " << readShadowFuncName << "(shadowTexture" << i << ", gl_TexCoord[shadowTextureUnit" << i << "]);\n";
    }
    
    ss << "  color *= mix(colorAmbientEmissive, gl_Color, shadowTerm);\n"
       << "  gl_FragColor = color;\n"
       << "}\n";
    return ss.str();
}

ShadowSettings::ShadowSettings():
    _lightNum(-1),
    _baseShadowTextureUnit(1),
    _useShadowMapTextureOverride(true),
    _textureSize(2048,2048),
    _minimumShadowMapNearFarRatio(0.05),
    _maximumShadowMapNearFarDistance(DBL_MAX),
    _perspectiveShadowMapCutOffAngle(2.0),
    _numShadowMapsPerLight(1),
    _shadowMapTechniqueHint(PARALLEL_SPLIT|PERSPECTIVE),
    _shaderHint(NO_SHADERS),
    //_shaderHint(PROVIDE_FRAGMENT_SHADER),
    _debugDraw(false)
{
    _shaderGenerator = new ShaderGenerator;
}

ShadowSettings::ShadowSettings(const ShadowSettings& ss, const osg::CopyOp& copyop):
    Object(ss,copyop),
    _shaderGenerator(ss._shaderGenerator),
    _lightNum(ss._lightNum),
    _baseShadowTextureUnit(ss._baseShadowTextureUnit),
    _useShadowMapTextureOverride(ss._useShadowMapTextureOverride),
    _textureSize(ss._textureSize),
    _minimumShadowMapNearFarRatio(ss._minimumShadowMapNearFarRatio),
    _maximumShadowMapNearFarDistance(ss._maximumShadowMapNearFarDistance),
    _perspectiveShadowMapCutOffAngle(ss._perspectiveShadowMapCutOffAngle),
    _numShadowMapsPerLight(ss._numShadowMapsPerLight),
    _shadowMapTechniqueHint(ss._shadowMapTechniqueHint),
    _shaderHint(ss._shaderHint),
    _debugDraw(ss._debugDraw)
{
}

ShadowSettings::~ShadowSettings()
{
}
