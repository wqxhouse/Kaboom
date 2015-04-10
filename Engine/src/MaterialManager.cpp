#include "stdafx.h" 
#include "MaterialManager.h"
#include "Material.h"

MaterialManager::MaterialManager()
{
	createBuiltInMaterials();
}

MaterialManager::~MaterialManager()
{
	// clear built in materials
	for (int i = 0; i < _builtInMaterial.size(); i++)
	{
		delete _builtInMaterial[i];
	}

	_builtInMaterial.clear();

	// clear created materials
	for (std::unordered_map<std::string, Material *>::iterator it = _materialMap.begin();
		it != _materialMap.end(); ++it)
	{
		delete it->second;
	}

	_materialMap.clear();

	// TODO: make sure if 'deleting' the texture ref necessary
	_textureMap.clear();

	_materialGeomBinding.clear();

}

void MaterialManager::createBuiltInMaterials()
{
	// default
	Material *m = new Material("____builtInDefault", onTexturePathChange);
	_builtInMaterial.push_back(m);
}

Material *MaterialManager::getBuiltInMaterial(enum MaterialBuiltIn type)
{
	return _builtInMaterial[(int)type];
}

void MaterialManager::createPlainMaterial(const std::string &name,
	const osg::Vec3 albedoColor,
	float roughness,
	float specular,
	float metallic)
{
	Material *mat = new Material(name, onTexturePathChange);
	mat->setAlbedo(albedoColor);
	mat->setRoughness(roughness);
	mat->setSpecular(specular);
	mat->setMetallic(metallic);

	_materialMap.insert(std::make_pair(name, mat));
}

void MaterialManager::createTextureMaterial(const std::string &name,
	const std::string albedoPath,
	const std::string roughnessPath,
	const std::string specularPath,
	const std::string metallicPath)
{
	// TODO: implement	
}


Material *MaterialManager::getMaterial(const std::string &name)
{
	std::unordered_map<std::string, Material *>::iterator it = _materialMap.find(name);
	if (it != _materialMap.end())
	{
		return it->second;
	}
	else
	{
		// console output material not found;
		return NULL;
	}
}


void MaterialManager::onTexturePathChange(const std::string &texturePath)
{
	// TODO: implement - reload texture , or load cached if path matches
}