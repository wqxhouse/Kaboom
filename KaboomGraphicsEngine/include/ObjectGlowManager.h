#pragma once
#include <vector>
#include <osg/Group>
#include "GeometryObject.h"

class ObjectGlowManager
{
public:
	ObjectGlowManager();

	void addGlowGeometryObject(GeometryObject *geomObj);
	void removeGlowGeometryObject(GeometryObject *geomObj);
	inline osg::ref_ptr<osg::Group> getRoot()
	{
		return _glowObjectGroup;
	}

private:
	GeometryObject *_glowGeometry;

	// std::vector<GeometryObject *> _glowObjects;
	osg::ref_ptr<osg::Group> _glowObjectGroup;
	osg::ref_ptr<osg::Program> _shader;
};