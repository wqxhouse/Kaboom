#pragma once
#include <osg/StateSet>
#include <CubeMapPreFilter.h>

class LightPassCallback : public osg::StateSet::Callback
{
public:
	LightPassCallback(CubeMapPreFilter *cubemapPreFilter);
	~LightPassCallback();

	virtual void operator()(osg::StateSet *ss, osg::NodeVisitor *nv);

private:
	CubeMapPreFilter *_cubemapPreFilter;
};

