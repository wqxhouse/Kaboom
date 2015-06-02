// Zonglin (Robin) Wu 3/31 
// World - Provide support for scene management
// providing interface for spatial partitioning
// and object manipulation through in-game editor

#pragma once

#include <string>
#include <unordered_map>
#include <osg/Node>
#include <osgDB/XmlParser>
#include <util/XMLLoader.h>

class LightManager;
class MaterialManager;
class GeometryObjectManager;
class ParticleEffectManager;
class ObjectGlowManager;
class World : public XMLLoader
{
public:
	World();
	~World();

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

	inline ParticleEffectManager *getParticleEffectManager()
	{
		return _particleEffectManager;
	}

	inline ObjectGlowManager *getObjectGlowManager()
	{
		return _objectGlowManager;
	}

protected:
	virtual void loadXMLNode(osgDB::XmlNode *xmlRoot);

private:
	GeometryObjectManager *_geomManager;
	MaterialManager *_materialManager;
	LightManager *_lightManager;
	ParticleEffectManager *_particleEffectManager;
	ObjectGlowManager *_objectGlowManager;
};

