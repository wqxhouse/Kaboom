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

bool GeometryObjectManager::addGeometry(const std::string &name, osg::Node *geomNode, osg::Vec3 pos)
{
	GeometryObject *geomObj = new GeometryObject(name, geomNode);
	geomObj->setTranslate(pos);

	// TODO: process duplicated (name) geoms
	_geomObjMap.insert(std::make_pair(name, geomObj));
	_geomRoot->addChild(geomObj->getRoot());
	return true;
}

bool GeometryObjectManager::addGeometry(const std::string &name, osg::Node *geomNode, std::string fileName)
{
	GeometryObject *geomObj = new GeometryObject(name, geomNode, fileName);

	// TODO: process duplicated (name) geoms
	_geomObjMap.insert(std::make_pair(name, geomObj));
	_geomRoot->addChild(geomObj->getRoot());
	return true;
}

void GeometryObjectManager::deleteGeometry(const std::string &name){
	GeometryObject *geomObj = _geomObjMap[name];
	
	_geomObjMap.erase(name);
	_geomRoot->removeChild(geomObj->getRoot());
	delete geomObj;
}

void GeometryObjectManager::renameGeometry(const std::string &oldName, const std::string newName){
	GeometryObject *geomObj = _geomObjMap[oldName];
	_geomObjMap.erase(oldName);

	geomObj->setName(newName);
	_geomObjMap.insert(std::make_pair(newName, geomObj));
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