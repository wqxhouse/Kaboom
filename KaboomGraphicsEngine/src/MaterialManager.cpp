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
	const std::string &albedoPath,
	const std::string &roughnessPath,
	const std::string &metallicPath,
	const std::string &normalMapPath)
{
	//osg::Image *albedo = osgDB::readImageFile(albedoPath);
	//osg::Image *specular = osgDB::readImageFile(specularPath);
	//osg::Image *roughness = osgDB::readImageFile(roughnessPath);
	//osg::Image *metallic = osgDB::readImageFile(metallicPath);
	//osg::Image *normalMap = osgDB::readImageFile(normalMapPath);

	//osg::Texture2D *albedoTex = new osg::Texture2D;
	//albedoTex->setImage(albedo);
	//_textureMap.insert(std::make_pair(albedoPath, albedoTex));

	//osg::Texture2D *specularTex = new osg::Texture2D;
	//specularTex->setImage(specular);
	//_textureMap.insert(std::make_pair(specularPath, specularTex));

	//osg::Texture2D *roughnessTex = new osg::Texture2D;
	//roughnessTex->setImage(roughness);
	//_textureMap.insert(std::make_pair(roughnessPath, roughnessTex));

	//osg::Texture2D *metallicTex = new osg::Texture2D;
	//metallicTex->setImage(metallic);
	//_textureMap.insert(std::make_pair(metallicPath, metallicTex));

	//osg::Texture2D *normalMapTex = new osg::Texture2D;
	//normalMapTex->setImage(normalMap);
	//_textureMap.insert(std::make_pair(normalMapPath, normalMapTex));

	// above unnecessary since set..Texture handles them
	Material *mat = new Material(name, onTexturePathChange);
	mat->setUseTexture(true);
	mat->setAlbedoTexturePath(albedoPath);
	mat->setMetallicMapPath(metallicPath);
	mat->setRoughnessMapPath(roughnessPath);
	mat->setNormalMapPath(normalMapPath);

	_materialMap.insert(std::make_pair(name, mat));
}

void MaterialManager::renameMaterial(const std::string &oldName, const std::string &newName)
{
	Material *mat = _materialMap[oldName];
	_materialMap.erase(oldName);

	mat->setName(newName);
	_materialMap.insert(std::make_pair(newName, mat));
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

	// TODO: delete changed texture, or memory leak!
	// not sure if ref_ptr automatically deallocates unreferenced image

	std::unordered_map<std::string, osg::ref_ptr<osg::Texture> > &texMap 
		= _weakMaterialManagerPtr->_textureMap;
	auto it = texMap.find(texturePath);
	// update only if not found
	if (it == texMap.end())
	{
		osg::Texture2D *tex = new osg::Texture2D;
		osg::Image *img = osgDB::readImageFile(texturePath);
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