#pragma once

#include <Windows.h>

#include <osg/NodeCallback>

class PlayerNodeCallback : public osg::NodeCallback {
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
};
