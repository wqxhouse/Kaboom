#pragma once
#include "Visualizer.h"
#include <osg/Group>

class AxisVisualizer : public Visualizer
{
public:
	AxisVisualizer();

private:
	void makeAxisGeode();
};