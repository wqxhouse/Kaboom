#include "stdafx.h" 
#include "Material.h"

Material::Material(const std::string &name, TextureChangeCallback onTextureChange)
	: _useTexture(false), _name(name), _onTextureChangeFunc(onTextureChange), 
	_albedo(osg::Vec3(0.1f, 0.3f, 0.9f)), _roughness(0.5), _specular(0.5), _metallic(0.0), 
	_albedoTexLerp(0.0), _roughnessTexLerp(0.0), _metallicTexLerp(0.0), _normalMapLerp(0.0), 
	_textureOffset(), _updateCallback(NULL), _materialUpdateEnabled(false)
{
}

Material::~Material()
{
}
