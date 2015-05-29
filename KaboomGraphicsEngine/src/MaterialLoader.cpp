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
	float roughness = 0.0, specular = 0.0, metallic = 0.0;

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

	std::string albedoTex, roughnessTex, metallicTex, normalPath;

	for (unsigned int i = 0; i < xmlNode->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlNode->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;

		if (childName == "albedoTex") {
			loadString(xmlChild, albedoTex);
		}
		else if (childName == "roughnessTex") {
			loadString(xmlChild, roughnessTex);
		}
		else if (childName == "metallicTex") {
			loadString(xmlChild, metallicTex);
		}
		else if (childName == "normalPath") {
			loadString(xmlChild, normalPath);
		}
	}

	setDefaultString(albedoTex);
	setDefaultString(roughnessTex);
	setDefaultString(metallicTex);
	setDefaultString(normalPath);

	_materialManager->createTexturedMaterial(name,
		albedoTex, roughnessTex, metallicTex, normalPath);
}
