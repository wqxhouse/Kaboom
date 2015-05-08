
#pragma once

#include <string>
#include <unordered_map>
#include <osg/Node>
#include <osgDB/XmlParser>
#include <util/XMLLoader.h>

//class MaterialManager;
class TypeIdLoader : public XMLLoader
{
public:
	TypeIdLoader();

	void createTypeFromXML(osgDB::XmlNode* xmlNode);

protected:
	virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

private:

	//MaterialManager *_materialManager;
};