#include "TypeIdLoader.h"

#include <osgDB/ReadFile>
#include "stdafx.h" 

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/Options>
#include <osgDB/XmlParser>

TypeIdLoader::TypeIdLoader()
{
	
}

void TypeIdLoader::loadXMLNode(osgDB::XmlNode *xmlRoot)
{
	// Ignore the most outer tag ("world")
	if (xmlRoot->type == osgDB::XmlNode::ROOT)
	{
		for (unsigned int i = 0; i < xmlRoot->children.size(); ++i)
		{
			osgDB::XmlNode* xmlChild = xmlRoot->children[i];
			if (xmlChild->name == "dynamic_objects")
				return loadXMLNode(xmlChild);
		}
	}

	for (unsigned int i = 0; i < xmlRoot->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlRoot->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;
		//std::cout << childName << std::endl;

		if (childName == "type") {
			createTypeFromXML(xmlChild);
		}
	}
}

void TypeIdLoader::createTypeFromXML(osgDB::XmlNode* xmlNode)
{
	std::string id = xmlNode->properties["id"];

	std::string meshPath, materialName;

	for (unsigned int i = 0; i < xmlNode->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlNode->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;

		if (childName == "mesh") {
			loadString(xmlChild, meshPath);
		}
		else if (childName == "material") {
			materialName = xmlChild->properties["name"];
		}
	}

	setDefaultString(meshPath);
	setDefaultString(materialName);

	/*
	_materialManager->createTextureMaterial(name,
		albedoTex, roughnessTex, metallicTex, normalPath);*/
}
