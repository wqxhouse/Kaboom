#include "World.h"

#include <osgDB/ReadFile>
#include "stdafx.h" 

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/Options>
#include <osgDB/XmlParser>

#include "GeometryCache.h"
#include "GeometryObject.h"
#include "GeometryObjectManager.h"
#include "LightManager.h"
#include "MaterialManager.h"
#include "ParticleEffectManager.h"
#include "ObjectGlowManager.h"
#include "CameraShakeManager.h"

World::World()
{
	_geomManager = new GeometryObjectManager;
	_materialManager = new MaterialManager;
	_lightManager = new LightManager;
	_particleEffectManager = new ParticleEffectManager;
	_cache = new GeometryCache;
	_objectGlowManager = new ObjectGlowManager;
	_cameraShakeManager = new CameraShakeManager;
}

World::~World()
{
	delete _geomManager;
	delete _materialManager;
	delete _lightManager;
	delete _particleEffectManager;
	delete _cache;
}

void World::loadXMLFile(const std::string &filePath)
{
	XMLLoader::loadXMLFile(filePath);

	std::size_t found = filePath.find("\\World\\");
	_worldPath = filePath.substr(0, found);
}

void World::loadXMLNode(osgDB::XmlNode *xmlRoot)
{
	// Ignore the most outer tag ("world")
	if (xmlRoot->type == osgDB::XmlNode::ROOT)
	{
		for (unsigned int i = 0; i < xmlRoot->children.size(); ++i)
		{
			osgDB::XmlNode* xmlChild = xmlRoot->children[i];
			if (xmlChild->name == "world")
				return loadXMLNode(xmlChild);
		}
	}

	for (unsigned int i = 0; i < xmlRoot->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlRoot->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;
		//std::cout << childName << std::endl;

		if (childName == "model") {
			createModelFromXML(xmlChild);
		}
		else if (childName == "light") {
			createLightFromXML(xmlChild);
		}
	}
}

void World::createModelFromXML(osgDB::XmlNode* xmlNode)
{
	std::string name = xmlNode->properties["name"];
	osg::Node *model = nullptr;
	osg::Vec3 position;
	osg::Vec4 orientation;
	osg::Vec3 scale;

	for (unsigned int i = 0; i < xmlNode->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlNode->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;

		if (childName == "file") {
			std::string file;
			loadString(xmlChild, file);

			model = _cache->getNodeByFileName(file);
			if (!_geomManager->addGeometry(name, model, file)) {
				std::cout << "createModelFromXML(): " << name << " failed to create, trying to find if it exists already..." << std::endl;
				if (_geomManager->getGeometryObject(name) != NULL) {
					std::cout << "  => found " + name + " in the GeometryObjectManager" << std::endl;
					continue;
				} else {
					std::cout << "  => not found in GeometryObjectManager... break." << std::endl;
				}
			}
		}
		else if (childName == "material") {
			std::string mat = xmlChild->properties["name"];
			Material *m = _materialManager->getMaterial(mat);

			if (m != nullptr) {
				_geomManager->setGeometryMaterial(name, m);
			}
		}
		else if (childName == "position") {
			loadVec3(xmlChild, position);

			osg::Matrix translate;
			translate.makeTranslate(position);

			GeometryObject* tmp;
			tmp = _geomManager->getGeometryObject(name);
			tmp->setTranslate(position);

		}
		else if (childName == "orientation") {
			loadVec4(xmlChild, orientation);

			osg::Quat quat = osg::Quat(orientation);

			GeometryObject* tmp;
			tmp = _geomManager->getGeometryObject(name);
			tmp->setRotation(quat);
		}
		else if (childName == "scale") {
			loadVec3(xmlChild, scale);

			GeometryObject* tmp;
			tmp = _geomManager->getGeometryObject(name);
			tmp->setScale(scale);
			//tmp->scale(osg::Vec3(1.0f, 1.0f, 1.0f));

		}
		//else if (childName == "collider") {
		//	std::stringstream ss; ss << xmlChild->getTrimmedContents();
		//	ss >> metallic;
		//}
	}
}

void World::createLightFromXML(osgDB::XmlNode* xmlNode)
{
	std::string name = xmlNode->properties["name"];
	std::string type = xmlNode->properties["type"];
	osg::Vec3 color, position, direction;
	float radius, intensity = 1.0f;
	bool doShadow;

	for (unsigned int i = 0; i < xmlNode->children.size(); ++i)
	{
		osgDB::XmlNode* xmlChild = xmlNode->children[i];
		if (!isXMLNodeType(xmlChild)) continue;

		const std::string& childName = xmlChild->name;

		if (childName == "color") {
			loadVec3(xmlChild, color);
		}
		else if (childName == "position") {
			loadVec3(xmlChild, position);
		}
		else if (childName == "direction") {
			loadVec3(xmlChild, direction);
		}
		else if (childName == "intensity") {
			loadFloat(xmlChild, intensity);
		}
		else if (childName == "radius") {
			loadFloat(xmlChild, radius);
		}
		else if (childName == "castShadow") {
			loadBool(xmlChild, doShadow);
		}
	}

	if (type == "point") {
		_lightManager->addPointLight(name, position, color, radius, doShadow, intensity);
	}
	else if (type == "directional") {
		_lightManager->addDirectionalLight(name, direction, color, doShadow, intensity);
	}
}
