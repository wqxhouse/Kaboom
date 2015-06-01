#pragma once

#include <vector>
#include <osg/Camera>
#include <osg/TextureCubeMap>
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
	void removePointLight(PointLight *light);

	inline osg::ref_ptr<osg::TextureCubeMap> getShadowFaceLookupCubeTex()
	{
		return _pointLightShadowFaceLookupTex;
	}

	inline osg::ref_ptr<osg::Texture2D> getShadowAtlas()
	{
		return _depthAtlasTex;
	}

	inline osg::ref_ptr<osg::Group> getRoot()
	{
		return _depthCamGroup;
	}
	
	inline int getCurrShadowMapNum()
	{
		return _currShadowMapNum;
	}

	int getNumShadowMaps();
	bool hasCameraAtIndex(int index);
	const osg::Matrix &getLightSpaceWVP(int index);
	float getShadowMapScaleWRTAtlas(int index);
	osg::Vec2 getAtlasPosUVCoord(int index);

private:
	void getPassInfo();
	void configShadowMapFaceLookupCube();
	int findAvailableDepthSlot();
	bool checkHasEnoughSlotForPointLightShadow();

	ShadowAtlas *_atlas;
	osg::observer_ptr<osgFX::EffectCompositor> _passes;
	// osg::observer_ptr<osg::Camera> _shadowDepthPassCam;
	osg::ref_ptr<osg::Group> _depthCamGroup;
	osg::ref_ptr<osg::TextureCubeMap> _pointLightShadowFaceLookupTex;

	std::vector<ShadowDepthCamera *> _depthCameras;
	osg::observer_ptr<osg::Texture2D> _depthAtlasTex;

	osg::observer_ptr<osg::Group> _geomRoot;

	// used for getting reference when depthCamera by index not found
	// used by const osg::Matrix &getLightSpaceWVP(int index);
	osg::Matrix _defaultMatrix; 

	int _currShadowMapNum;
};

class DepthCamGroupCallback : public osg::NodeCallback
{
public:
	DepthCamGroupCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
private:
	LightManager *_lmanager;
};