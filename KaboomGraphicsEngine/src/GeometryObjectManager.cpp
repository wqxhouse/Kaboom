#include "stdafx.h" 

#include "GeometryObjectManager.h"
#include "GeometryObject.h"
#include "GeometryCache.h"
#include "Core.h"

GeometryObjectManager::GeometryObjectManager()
{
	_geomRoot = new osg::Group;
	_suffix = 1;
	_shadowedScene = new osg::Group;
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

bool GeometryObjectManager::addGeometry(const std::string &name, osg::Node *geomNode, osg::Vec3 pos, bool receiveShadow)
{
	if (geomNode == nullptr) {
		std::cout << "geomNode is null: " << name << std::endl;
		return false;
	}

	// Handle duplicated (name) geoms
	if (doesNameExist(name)) {
		std::cout << "Name already exists: " << name << std::endl;
		return false;
	}

	GeometryObject *geomObj = new GeometryObject(name, geomNode);
	geomObj->setTranslate(pos);

	_geomObjMap.insert(std::make_pair(name, geomObj));
	_geomRoot->addChild(geomObj->getRoot());

	if (receiveShadow)
	{
		_shadowedScene->addChild(geomObj->getRoot());
		geomObj->setReceiveShadow(true);
	}

	return true;
}

bool GeometryObjectManager::addGeometry(const std::string &name, osg::Node *geomNode, std::string fileName, osg::Vec3 pos, bool receiveShadow)
{
	if (geomNode == nullptr) {
		std::cout << "geomNode is null: " << name << std::endl;
		return false;
	}

	// Handle duplicated (name) geoms
	if (doesNameExist(name)) {
		std::cout << "Name already exists: " << name << std::endl;
		return false;
	}

	GeometryObject *geomObj = new GeometryObject(name, geomNode, fileName);
	geomObj->setTranslate(pos);

	_geomObjMap.insert(std::make_pair(name, geomObj));
	_geomRoot->addChild(geomObj->getRoot());

	if (receiveShadow)
	{
		_shadowedScene->addChild(geomObj->getRoot());
		geomObj->setReceiveShadow(true);
	}


	return true;
}

bool GeometryObjectManager::addGeometryByTypeId(const std::string &name, const int type_id, osg::Vec3 pos, bool receiveShadow)
{
	GeometryCache* cache = Core::getWorldRef().getGeometryCache();
	std::string fileName;

	osg::ref_ptr<osg::Node> geomNode = cache->getNodeById(type_id, fileName);
	Material* mat = cache->getMaterialById(type_id);

	if ((geomNode.get() == NULL) || (mat == NULL)) 
	{
		return false;
	}

	return addGeometry(name, geomNode, fileName, pos);
}

void GeometryObjectManager::deleteGeometry(const std::string &name)
{
	GeometryObject *geomObj = _geomObjMap[name];
	if (_geomObjMap.find(name) != _geomObjMap.end()){
		_geomObjMap.erase(name);
		_geomRoot->removeChild(geomObj->getRoot());

		if (geomObj->getReceiveShadow())
		{
			_shadowedScene->removeChild(geomObj->getRoot());
		}
		delete geomObj;
	}
}

bool GeometryObjectManager::renameGeometry(const std::string &oldName, const std::string newName)
{
	// Handle duplicated (name) geoms
	if (doesNameExist(newName)) {
		std::cout << "Name already exists: " << newName << std::endl;
		return false;
	}

	GeometryObject *geomObj = _geomObjMap[oldName];
	_geomObjMap.erase(oldName);

	geomObj->rename(newName);
	_geomObjMap.insert(std::make_pair(newName, geomObj));

	return true;
}

GeometryObject* GeometryObjectManager::copyGeometry(const std::string &name)
{
	GeometryObject* geomToCopy = getGeometryObject(name);
	if (geomToCopy == NULL) return NULL;

	// Handle duplicated (name) geoms
	std::string newName = name;
	while (doesNameExist(newName)) {
		newName = newName + std::to_string(_suffix++);
	}

	GeometryObject* newGeom = geomToCopy->copy(newName);
	_geomObjMap.insert(std::make_pair(newName, newGeom));
	_geomRoot->addChild(newGeom->getRoot());

	return newGeom;
}

bool GeometryObjectManager::doesNameExist(const std::string &name)
{
	std::unordered_map<std::string, GeometryObject *>::const_iterator got =
		_geomObjMap.find(name);

	// If the name already exists
	if (got != _geomObjMap.end()) {
		return true;
	}
	return false;
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
		OSG_WARN << "Geom object " << geomName << " not found in the geometryManager" << std::endl;
		return NULL;
	}
}