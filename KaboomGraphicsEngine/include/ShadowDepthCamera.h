#pragma once
#include <osg/Camera>
#include <osg/StateSet>

class Light;
class PointLight;
class DirectionalLight;
class ShadowDepthCameraCallback;
class ShadowAtlas;
class ShadowDepthCamera
{
public:
	ShadowDepthCamera(osg::Texture2D *shadowAtlasTex, ShadowAtlas *atlas, 
		osg::Group *geomRoot, Light *light, int face = -1);
	void setActive(bool tf);
	inline osg::ref_ptr<osg::Camera> getRoot()
	{
		return _shadowDepthCam;
	}

private:

	// void createSharedQuad();
	// osg::Matrix getCurrWVPById(int id);

	osg::ref_ptr<osg::Camera> _shadowDepthCam;
	osg::ref_ptr<osg::Texture2D> _shadowAtlasTex;

	int _id;
	int _face; // for point light depth cube face

	Light *_light;
	ShadowDepthCameraCallback *_updateCallback;
	ShadowAtlas *_atlas;
	osg::observer_ptr<osg::Group> _geomRoot;
	static int _highest_id;
};

class ShadowDepthCameraCallback : public osg::NodeCallback
{
public:
	ShadowDepthCameraCallback(ShadowAtlas *atlas, Light *light, int face=-1);
	virtual void operator()(osg::Node *, osg::NodeVisitor *);

	inline osg::Matrix getCurrWVP()
	{
		return _currWVP;
	}

private:
	// void setRegion(float l, float r, float b, float t);
	void makeLightSpaceMat(osg::Matrix &view, osg::Matrix &proj);
	osg::Matrix calcPointLightViewMat(const osg::Vec3 &pos, int face);
	void setShadowMapResHelper(Light *light, int resolution);
	void setShadowMapAtlasPosHelper(Light *light, const osg::Vec2i &atlasPos, int face=-1);

	Light *_light;
	int _face;
	osg::Matrix _currWVP;
	ShadowAtlas *_atlas;
};