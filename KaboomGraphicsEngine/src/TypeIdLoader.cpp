#include "TypeIdLoader.h"

#include "Core.h"
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
	std::string id_str = xmlNode->properties["id"];
	int id = std::stoi(id_str);

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

	// Get the material
	Material* mat = Core::getWorldRef().getMaterialManager()->getMaterial(materialName);

	// Load the model
	osg::Node *model = nullptr;
	model = osgDB::readNodeFile(meshPath);

	// Pre-load geometry to manager
	Core::getWorldRef().getGeometryManager()->storeTypeIdGeometry(id, model, meshPath, mat);
}
