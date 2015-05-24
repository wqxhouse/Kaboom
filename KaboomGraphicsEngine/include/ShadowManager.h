#pragma once

#include <vector>
#include <osg/Camera>
#include "ShadowAtlas.h"
#include "ShadowDepthCamera.h"

namespace osgFX
{
	class EffectCompositor;
}

class Light;
class DirectionalLight;
class PointLight;
class LightManager;
class ShadowManager
{
public:
	ShadowManager(osgFX::EffectCompositor *passes, osg::Group *geomRoot);
	~ShadowManager();

	void addDirectionalLight(DirectionalLight *light);
	void addPointLight(PointLight *light);

	inline osg::ref_ptr<osg::Group> getRoot()
	{
		return _depthCamGroup;
	}

private:
	void getPassInfo();
	int findAvailableDepthSlot();

	ShadowAtlas *_atlas;
	osg::observer_ptr<osgFX::EffectCompositor> _passes;
	// osg::observer_ptr<osg::Camera> _shadowDepthPassCam;
	osg::ref_ptr<osg::Group> _depthCamGroup;

	std::vector<ShadowDepthCamera *> _depthCameras;
	osg::observer_ptr<osg::Texture2D> _depthAtlasTex;

	osg::observer_ptr<osg::Group> _geomRoot;
};