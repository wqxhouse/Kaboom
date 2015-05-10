#pragma once
#include "Visualizer.h"
#include <osg/Group>
#include <osg/Node>

class LightVisualizer : public Visualizer
{
public:
	LightVisualizer();
	LightVisualizer(const osg::Vec3 &pos);

private:
	void makeLightGeode();

};
