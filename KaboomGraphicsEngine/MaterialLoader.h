
#pragma once

#include <string>
#include <unordered_map>
#include <osg/Node>
#include <osgDB/XmlParser>
#include <util/XMLLoader.h>

class MaterialManager;
class MaterialLoader : public XMLLoader
{
public:
	MaterialLoader(MaterialManager* mm);

	void createPlainMaterialFromXML(osgDB::XmlNode* xmlNode);
	void createTexturedMaterialFromXML(osgDB::XmlNode* xmlNode);

	inline MaterialManager *getMaterialManager()
	{
		return _materialManager;
	}

protected:
	virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

private:

	MaterialManager *_materialManager;
};