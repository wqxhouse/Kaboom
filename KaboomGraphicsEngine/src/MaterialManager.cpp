#include "stdafx.h" 
#include "MaterialManager.h"
#include "Material.h"

MaterialManager *MaterialManager::_weakMaterialManagerPtr;
MaterialManager::MaterialManager()
{
	_weakMaterialManagerPtr = this;
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

bool MaterialManager::createPlainMaterial(const std::string &name,
	const osg::Vec3 albedoColor,
	float roughness,
	float specular,
	float metallic)
{
	// Handle duplicated (name) geoms
	if (doesNameExist(name)) {
		std::cout << "Name already exists: " << name << std::endl;
		return false;
	}

	Material *mat = new Material(name, onTexturePathChange);
	mat->setAlbedo(albedoColor);
	mat->setRoughness(roughness);
	mat->setSpecular(specular);
	mat->setMetallic(metallic);

	_materialMap.insert(std::make_pair(name, mat));

	return true;
}

bool MaterialManager::createTextureMaterial(const std::string &name,
	const std::string &albedoPath,
	const std::string &roughnessPath,
	const std::string &metallicPath,
	const std::string &normalMapPath, 
	osg::Texture::WrapMode mode)
{
	// Handle duplicated (name) geoms
	if (doesNameExist(name)) {
		std::cout << "Name already exists: " << name << std::endl;
		return false;
	}

	Material *mat = new Material(name, onTexturePathChange);
	mat->setUseTexture(true);
	mat->setMode(mode);

	
	mat->setAlbedoTexturePath(albedoPath, mode);
	mat->setMetallicMapPath(metallicPath, mode);
	mat->setRoughnessMapPath(roughnessPath, mode);
	mat->setNormalMapPath(normalMapPath, mode);

	_materialMap.insert(std::make_pair(name, mat));

	return true;
}

bool MaterialManager::renameMaterial(const std::string &oldName, const std::string &newName)
{
	// Handle duplicated (name) geoms
	if (doesNameExist(newName)) {
		std::cout << "Name already exists: " << newName << std::endl;
		return false;
	}

	Material *mat = _materialMap[oldName];
	_materialMap.erase(oldName);

	mat->setName(newName);
	_materialMap.insert(std::make_pair(newName, mat));

	return true;
}

bool MaterialManager::doesNameExist(const std::string &name)
{
	std::unordered_map<std::string, Material *>::const_iterator got =
		_materialMap.find(name);

	// If the name already exists
	if (got != _materialMap.end()) {
		return true;
	}
	return false;
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


void MaterialManager::onTexturePathChange(const std::string &texturePath, osg::Texture::WrapMode mode)
{
	// TODO: implement - reload texture , or load cached if path matches

	// TODO: delete changed texture, or memory leak!
	// not sure if ref_ptr automatically deallocates unreferenced image

	std::unordered_map<std::string, osg::ref_ptr<osg::Texture> > &texMap 
		= _weakMaterialManagerPtr->_textureMap;
	auto it = texMap.find(texturePath);
	// update only if not found
	if (it == texMap.end())
	{
		osg::Texture2D *tex = new osg::Texture2D;
		tex->setWrap(osg::Texture::WRAP_S, mode);
		tex->setWrap(osg::Texture::WRAP_T, mode);
		OSG_WARN << "MaterialManager:: Reading image... " << texturePath;
		osg::Image *img = osgDB::readImageFile(texturePath);
		if (img == NULL)
		{
			OSG_WARN << " failed, may crash the program" << std::endl;
		}
		else
		{
			OSG_WARN << " successfully! " << std::endl;
		}
		tex->setImage(img);
		texMap.insert(std::make_pair(texturePath, tex));
	}
}

osg::ref_ptr<osg::Texture> MaterialManager::getAlbedoTexture(Material *m)
{
	auto it = _textureMap.find(m->getAlbedoTexturePath());
	if (it != _textureMap.end())
	{
		return it->second;
	}
	else
	{
		// console output material not found;
		return NULL;
	}
}

osg::ref_ptr<osg::Texture> MaterialManager::getRoughnessTexture(Material *m)
{
	auto it = _textureMap.find(m->getRoughnessTexturePath());
	if (it != _textureMap.end())
	{
		return it->second;
	}
	else
	{
		// console output material not found;
		return NULL;
	}
}

osg::ref_ptr<osg::Texture> MaterialManager::getMetallicTexture(Material *m)
{
	auto it = _textureMap.find(m->getMetallicTexturePath());
	if (it != _textureMap.end())
	{
		return it->second;
	}
	else
	{
		// console output material not found;
		return NULL;
	}
}

osg::ref_ptr<osg::Texture> MaterialManager::getNormalMapTexture(Material *m)
{
	auto it = _textureMap.find(m->getNormalMapTexturePath());
	if (it != _textureMap.end())
	{
		return it->second;
	}
	else
	{
		// console output material not found;
		return NULL;
	}
}