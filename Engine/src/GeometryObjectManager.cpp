#include "stdafx.h" 

#include "GeometryObjectManager.h"
#include "GeometryObject.h"

GeometryObjectManager::GeometryObjectManager()
{
	_geomRoot = new osg::Group;
}

GeometryObjectManager::~GeometryObjectManager()
{
	for (std::unordered_map<std::string, GeometryObject *>::iterator it = _geomObjMap.begin(); 
		it != _geomObjMap.end(); ++it)
	{
		delete it->second;
	}
	_geomObjMap.clear();
}

bool GeometryObjectManager::addGeometry(const std::string &name, osg::Node *geomNode)
{
	GeometryObject *geomObj = new GeometryObject(name, geomNode);

	// TODO: process duplicated (name) geoms
	_geomObjMap.insert(std::make_pair(name, geomObj));
	_geomRoot->addChild(geomObj->getRoot());
	return true;
}

bool GeometryObjectManager::setGeometryMaterial(const std::string &geomName, Material *material)
{
	if (material == NULL)
	{
		OSG_WARN << "setGeometryMaterial: Material is null" << std::endl;
	}

	std::unordered_map<std::string, GeometryObject *>::iterator itr = _geomObjMap.find(geomName);
	if (itr != _geomObjMap.end())
	{
		itr->second->setMaterial(material);
		return true;
	}
	else
	{
		// TODO: console output, need to implement text hud or something
		OSG_WARN << "setGeometryMaterial: geometry named " + geomName + " not defined" << std::endl;
		return false;
	}
}

GeometryObject *GeometryObjectManager::getGeometryObject(const std::string& geomName)
{
	std::unordered_map<std::string, GeometryObject *>::iterator itr = _geomObjMap.find(geomName);
	if (itr != _geomObjMap.end())
	{
		return itr->second;
	}
	else
	{
		// TODO: console output, need to implement text hud or something
		return NULL;
	}
}