#pragma once
#include <osg/StateSet>

class LightPassCallback : public osg::StateSet::Callback
{
public:
	LightPassCallback();
	~LightPassCallback();

	virtual void operator()(osg::StateSet *ss, osg::NodeVisitor *nv);
};

