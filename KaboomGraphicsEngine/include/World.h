// Zonglin (Robin) Wu 3/31 
// World - Provide support for scene management
// providing interface for spatial partitioning
// and object manipulation through in-game editor

#pragma once

#include <string>
#include <unordered_map>
#include <osg/Node>
#include <osgDB/XmlParser>

class LightManager;
class MaterialManager;
class GeometryObjectManager;
class World
{
public:
	World();
	~World();

	// TODO: implement it
	void loadWorldXMLFile(const std::string &worldFilePath);

	void createPlainMaterialFromXML(osgDB::XmlNode* xmlNode);
	void createTexturedMaterialFromXML(osgDB::XmlNode* xmlNode);
	void createModelFromXML(osgDB::XmlNode* xmlNode);
	void createLightFromXML(osgDB::XmlNode* xmlNode);

	inline GeometryObjectManager *getGeometryManager()
	{
		return _geomManager;
	}

	inline MaterialManager *getMaterialManager()
	{
		return _materialManager;
	}

	inline LightManager *getLightManager()
	{
		return _lightManager;
	}

private:
	bool isXMLNodeType(osgDB::XmlNode* xmlNode);
	void loadXMLNode(osgDB::XmlNode *xmlRoot);
	void setDefaultString(std::string &s);

	void loadInt(osgDB::XmlNode* xmlNode, int& i);
	void loadFloat(osgDB::XmlNode* xmlNode, float& f);
	void loadBool(osgDB::XmlNode* xmlNode, bool& b);
	void loadString(osgDB::XmlNode* xmlNode, std::string& s);
	void loadVec3(osgDB::XmlNode* xmlNode, osg::Vec3& vec);
	void loadVec4(osgDB::XmlNode* xmlNode, osg::Vec4& vec);
		
	GeometryObjectManager *_geomManager;
	MaterialManager *_materialManager;
	LightManager *_lightManager;
};

