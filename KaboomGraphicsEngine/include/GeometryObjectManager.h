// Zonglin (Robin) Wu 4/2/2015
// Class GeometryObject 
// Geometry Object creation, query, manage materials

#pragma once
#include <unordered_map>
#include <osg/Node>

// TODO: add material update callback frame

class Material;
class GeometryObject;
class GeometryObjectManager
{
public:
	GeometryObjectManager();
	~GeometryObjectManager();

	// TODO: right now the receiveShadow is only used to determine shadow map bounds, quite hacky but we have no time. 
	bool addGeometry(const std::string &name, osg::Node *geomNode, osg::Vec3 pos = osg::Vec3(0, 0, 0), bool receiveShadow = true);
	bool addGeometry(const std::string &name, osg::Node *geomNode, std::string fileName, bool receiveShadow = true);
	bool addGeometryByTypeId(const std::string &name, const int type_id, osg::Vec3 pos = osg::Vec3(0, 0, 0), bool receiveShadow = true);

	bool setGeometryMaterial(const std::string &geomName, Material *material);
	void deleteGeometry(const std::string &name);
	bool renameGeometry(const std::string &oldName, const std::string newName);
	GeometryObject* copyGeometry(const std::string &name);

	bool storeTypeIdGeometry(const int type_id, osg::Node *geomNode, std::string fileName, Material* material);

	bool doesNameExist(const std::string &name);

	inline osg::ref_ptr<osg::Group> getShadowedSceneNode()
	{
		return _shadowedScene;
	}

	inline const std::unordered_map<std::string, GeometryObject *> getGeometryObjectMapRef() const
	{
		return _geomObjMap;
	}

	inline osg::ref_ptr<osg::Group> getRootNode()
	{
		return _geomRoot;
	}

	// TODO: think twice about this. Is this necessary? Bad exposing may lead to serious problems. 
	GeometryObject *getGeometryObject(const std::string &geomName);

private:
	std::unordered_map<std::string, GeometryObject *> _geomObjMap;
	std::unordered_map<int, GeometryObject *> _typeIdGeomMap;
	osg::ref_ptr<osg::Group> _geomRoot;
	int _suffix;

	osg::ref_ptr<osg::Group> _shadowedScene; // used for calc scene bound

	// TODO: add receive shadow flag when implementing shadow maps
};

