#pragma once
#include "Visualizer.h"
#include <osg/Group>

class AxisVisualizer : public Visualizer
{
public:
	AxisVisualizer();

private:
	void makeAxisGeode();
	void configCamera();
	osg::ref_ptr<osg::Camera> _cam;
};