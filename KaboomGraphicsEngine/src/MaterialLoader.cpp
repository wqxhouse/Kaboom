#include "MaterialLoader.h"

#include <osgDB/ReadFile>
#include "stdafx.h" 

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/Options>
#include <osgDB/XmlParser>

#include "MaterialManager.h"

MaterialLoader::MaterialLoader(MaterialManager* mm)
{
	_materialManager = mm;
}

void MaterialLoader::loadXMLNode(osgDB::XmlNode *xmlRoot)
{
	// Ignore the most outer tag ("world")
	if (xmlRoot->type == osgDB::XmlNode::ROOT)
	{
		for (unsigned int i = 0; i < xmlRoot->children.size(); ++i)
		{
			osgDB::XmlNode* xmlChild = xmlRoot->children[i];
			if (xmlChild->name == "materialList")
				return loadXMLNode(xmlChild);
		}
	}

	for (unsigned int i = 0; i < xmlRoot->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlRoot->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;
		//std::cout << childName << std::endl;

		if (childName == "material") {
			std::string type = xmlChild->properties["type"];

			if (type == "plain") {
				createPlainMaterialFromXML(xmlChild);
			}
			else if (type == "textured") {
				createTexturedMaterialFromXML(xmlChild);
			}
		}
	}
}

void MaterialLoader::createPlainMaterialFromXML(osgDB::XmlNode* xmlNode)
{
	std::string name = xmlNode->properties["name"];

    osg::Vec3 albedo;
	float roughness = 0.0f, specular = 0.0f, metallic = 0.0f;

	for (unsigned int i = 0; i < xmlNode->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlNode->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;

		if (childName == "albedo") {
			loadVec3(xmlChild, albedo);
		}
		else if (childName == "roughness") {
			loadFloat(xmlChild, roughness);
		}
		else if (childName == "specular") {
			loadFloat(xmlChild, specular);
		}
		else if (childName == "metallic") {
			loadFloat(xmlChild, metallic);
		}
	}

	_materialManager->createPlainMaterial(name, albedo, roughness, specular, metallic);
}

void MaterialLoader::createTexturedMaterialFromXML(osgDB::XmlNode* xmlNode)
{
	std::string name = xmlNode->properties["name"];

    // Check if a material with the same name already exists
    if (_materialManager->getMaterial(name) != NULL) {
        printf("(MaterialLoader) Material already exists: %s\n", name.c_str());
        return;
    }

	std::string albedoTex, roughnessTex, metallicTex, normalPath;
    osg::Vec3 albedo = osg::Vec3(0.1f, 0.3f, 0.9f);
	float roughness = 0.5f, specular = 0.5f, metallic = 0.0f,
        albedoTexLerp = 0.0f, roughnessTexLerp = 0.0f, metallicTexLerp = 0.0f, normalTexLerp = 0.0f;

    albedoTex = "";
	roughnessTex = "";
	metallicTex = "";
	normalPath = "";

    // Create a material first in case that not all properties are specified
    _materialManager->createTexturedMaterial(name,
        albedoTex, roughnessTex, metallicTex, normalPath);

    Material* material = _materialManager->getMaterial(name);
    osg::Texture::WrapMode mode = material->getMode();

	for (unsigned int i = 0; i < xmlNode->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlNode->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;

		if (childName == "albedoTex") {
			loadString(xmlChild, albedoTex);
            material->setAlbedoTexturePath(albedoTex, mode);
		}
		else if (childName == "roughnessTex") {
			loadString(xmlChild, roughnessTex);
            material->setRoughnessMapPath(roughnessTex, mode);
		}
		else if (childName == "metallicTex") {
			loadString(xmlChild, metallicTex);
            material->setMetallicMapPath(metallicTex, mode);
		}
		else if (childName == "normalPath") {
			loadString(xmlChild, normalPath);
            material->setNormalMapPath(normalPath, mode); 
		}
        else if (childName == "albedo") {
            loadVec3(xmlChild, albedo);
            material->setAlbedo(albedo);
        } 
        else if (childName == "roughness") {
            loadFloat(xmlChild, roughness);
            material->setRoughness(roughness);
        } 
        else if (childName == "specular") {
            loadFloat(xmlChild, specular);
            material->setSpecular(specular);
        } 
        else if (childName == "metallic") {
            loadFloat(xmlChild, metallic);
            material->setMetallic(metallic);
        } 
        else if (childName == "albedoTexLerp") {
            loadFloat(xmlChild, albedoTexLerp);
            material->setAlbedoTexLerp(albedoTexLerp);
        } 
        else if (childName == "roughnessTexLerp") {
            loadFloat(xmlChild, roughnessTexLerp);
            material->setRoughnessTexLerp(roughnessTexLerp);
        } 
        else if (childName == "metallicTexLerp") {
            loadFloat(xmlChild, metallicTexLerp);
            material->setMetallicTexLerp(metallicTexLerp);
        } 
        else if (childName == "normalTexLerp") {
            loadFloat(xmlChild, normalTexLerp);
            material->setNormalMapLerp(normalTexLerp);
        }
	}
}
