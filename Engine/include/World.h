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
	void loadXMLNode(osgDB::XmlNode *xmlRoot);
		
	GeometryObjectManager *_geomManager;
	MaterialManager *_materialManager;
	LightManager *_lightManager;
};

