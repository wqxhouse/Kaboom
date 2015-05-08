#pragma once
#include <osgManipulator/Dragger>
#include <osg/MatrixTransform>

#include "GeometryObject.h"

class DraggerUpdateCallback : public osgManipulator::DraggerTransformCallback
{
public:
	DraggerUpdateCallback(osg::observer_ptr<osg::MatrixTransform> transform, int handleCommandMask = HANDLE_ALL);

	void setNode(osg::observer_ptr<osg::MatrixTransform> transform);
	virtual bool receive(const osgManipulator::MotionCommand & cmd);

private:
	GeometryObject *_geomObj;
};
