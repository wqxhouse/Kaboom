#pragma once
#include <osg/StateSet>
#include <osg/Polytope>

class Light;
class LightManager;
class LightPrePassCallback : public osg::StateSet::Callback
{
public:
	LightPrePassCallback(LightManager *lightManager);
	~LightPrePassCallback();

	virtual void operator()(osg::StateSet *ss, osg::NodeVisitor *nv);
private:
	std::vector<Light *> performLightCulling();

	LightManager *_manager;
	std::vector<osg::Vec4> _frustumVertices;
};

