#include "stdafx.h" 
#include "Material.h"

Material::Material(const std::string &name, TextureChangeCallback onTextureChange)
	: _useTexture(false), _name(name), _onTextureChangeFunc(onTextureChange), 
	_albedo(osg::Vec3(0.1f, 0.3f, 0.9f)), _roughness(1.0), _specular(1.0), _metallic(1.0)
{
}

Material::~Material()
{
}