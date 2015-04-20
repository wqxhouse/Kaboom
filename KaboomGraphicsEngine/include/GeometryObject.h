#pragma once
#include <string>
#include <osg/MatrixTransform>
#include <osg/NodeCallback>
#include <osg/Program>
#include <osgManipulator/Dragger>
#include <osgManipulator/Selection>
#include "EffectCompositor.h"

class Material;
class GeometryObjectNodeUpadateCallback;
class GeometryObject
{
public:
	GeometryObject(const std::string &name, osg::Node *geomNode);
	~GeometryObject();

	std::string getName()
	{
		return _name;
	}

	void setMaterial(Material *material);
	void setTransform(const osg::Matrixf &transform);

	osg::Vec3 getTranslate();
	void setTranslate(const osg::Vec3 &translate);

	osg::Quat getRotation();
	void setRotation(const osg::Quat &rot);

	// For GUI control or script control
	inline void setGeometryUpdateCallback(void(*geomObjUpdateCallback)(GeometryObject *))
	{
		_geomObjUpdateCallback = geomObjUpdateCallback;
	}

	inline osg::ref_ptr<osg::MatrixTransform> getRoot()
	{
		return _objRoot;
	}

private:
	void setUpMaterialState();
	void updateMaterialState();
		
	std::string _name;
	Material *_material;

	osg::ref_ptr<osg::Group> _materialNode;
	osg::ref_ptr<osg::MatrixTransform> _objRoot;

	static osg::ref_ptr<osg::Program> _plainShader;
	static osg::ref_ptr<osg::Program> _texturedShader;
	static osg::ref_ptr<osg::Program> getPlainShader();
	static osg::ref_ptr<osg::Program> getTexturedShader();

	friend class GeometryObjectNodeUpadateCallback;
	void(*_geomObjUpdateCallback)(GeometryObject *geomObj);
};

class GeometryObjectNodeUpadateCallback : public osg::NodeCallback
{
public:
	GeometryObjectNodeUpadateCallback(GeometryObject *geomObj)
		: _geomObj(geomObj) {}

	virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
	{
		osg::ref_ptr<osg::MatrixTransform> matNode = static_cast<osg::MatrixTransform *>(node);

		// update change from GUI or scripts
		if (_geomObj->_geomObjUpdateCallback != NULL)
		{
			_geomObj->_geomObjUpdateCallback(_geomObj);
		}

		// update uniform
		_geomObj->updateMaterialState();

		traverse(node, nv);
	}

private:
	GeometryObject *_geomObj;
};

