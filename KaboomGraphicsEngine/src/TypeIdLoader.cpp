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
	osg::Matrix matrix = osg::Matrix::identity();
	osg::Vec3 position = osg::Vec3(0.0f, 0.0f, 0.0f);
	osg::Vec4 orientation = osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	osg::Vec3 scale = osg::Vec3(1.0f, 1.0f, 1.0f);

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
		else if (childName == "position") {
			loadVec3(xmlChild, position);
		}
		else if (childName == "orientation") {
			loadVec4(xmlChild, orientation);
		}
		else if (childName == "scale") {
			loadVec3(xmlChild, scale);
		}
	}

	setDefaultString(meshPath);
	setDefaultString(materialName);

	// Calculate matrix
	osg::Quat rot = osg::Quat(orientation);

	matrix.makeTranslate(position);
	matrix.preMult(osg::Matrix::rotate(rot));
	matrix.preMult(osg::Matrix::scale(scale));

	// Get the material
	Material* mat = Core::getWorldRef().getMaterialManager()->getMaterial(materialName);

	// Pre-load model (not GeometryObject) to cache
	Core::getWorldRef().getGeometryCache()->addTypeId(id, meshPath, mat, matrix);
}