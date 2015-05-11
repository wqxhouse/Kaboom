#pragma once
#include "Visualizer.h"
#include <osg/Group>
#include <osg/Node>
#include <osg/NodeCallback>

class AxisVisualizer : public Visualizer
{
public:
	AxisVisualizer();

	void init();

private:
	void makeAxisGeode();
	void configCamera();

	osg::ref_ptr<osg::Camera> _cam;
};

class AxisVisalizerCallback : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};