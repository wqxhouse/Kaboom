#pragma once

#include <string>

/*
 * To use this, make a subclass loader
 *    and define the function loadXMLNode()
 * See World.h and World.cpp (in GraphisEngine) for reference
 */
class XMLLoader
{
public:
	void loadXMLFile(const std::string &filePath);

protected:
	virtual void loadXMLNode(osgDB::XmlNode *xmlRoot) = 0;

	bool isXMLNodeType(osgDB::XmlNode* xmlNode);
	void setDefaultString(std::string &s);

	void loadInt(osgDB::XmlNode* xmlNode, int& i);
	void loadFloat(osgDB::XmlNode* xmlNode, float& f);
	void loadBool(osgDB::XmlNode* xmlNode, bool& b);
	void loadString(osgDB::XmlNode* xmlNode, std::string& s);
	void loadVec3(osgDB::XmlNode* xmlNode, osg::Vec3& vec);
	void loadVec4(osgDB::XmlNode* xmlNode, osg::Vec4& vec);

};
