#pragma once
#include <osgManipulator/Dragger>
#include <osg/MatrixTransform>

#include "GeometryObject.h"

class DraggerUpdateCallback : public osgManipulator::DraggerTransformCallback
{
public:
	DraggerUpdateCallback(osg::MatrixTransform *transform, int handleCommandMask = HANDLE_ALL);

	void setNode(osg::MatrixTransform *transform);
	virtual bool receive(const osgManipulator::MotionCommand & cmd);

private:
	std::string _geomName;
	osg::observer_ptr<osg::MatrixTransform> _matTrans;
};
