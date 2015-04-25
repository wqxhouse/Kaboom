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

	bool addGeometry(const std::string &name, osg::Node *geomNode, osg::Vec3 pos = osg::Vec3(0, 0, 0));
	bool addGeometry(const std::string &name, osg::Node *geomNode, std::string fileName);
	bool setGeometryMaterial(const std::string &geomName, Material *material);
	void deleteGeometry(const std::string &name);
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
	osg::ref_ptr<osg::Group> _geomRoot;

	// TODO: add receive shadow flag when implementing shadow maps
};

