#pragma once
#include <osg/StateSet>
#include <CubeMapPreFilter.h>

class LightManager;
class LightPassCallback : public osg::StateSet::Callback
{
public:
	LightPassCallback(CubeMapPreFilter *cubemapPreFilter, LightManager *manager);
	~LightPassCallback();

	virtual void operator()(osg::StateSet *ss, osg::NodeVisitor *nv);

private:
	struct ShadowMapUniformBlock
	{
		osg::Matrixf _vwvp;
		osg::Vec2 _atlas_uvcoord;
		float tex_scale;
	};

	CubeMapPreFilter *_cubemapPreFilter;
	LightManager *_manager;
};

