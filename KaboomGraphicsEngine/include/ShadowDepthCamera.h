#pragma once
#include <osg/Camera>
#include <osg/StateSet>

class Light;
class PointLight;
class DirectionalLight;
class ShadowDepthCameraCallback;
class ShadowDepthCamera
{
public:
	ShadowDepthCamera(osg::Texture2D *shadowAtlasTex, Light *light, int face=-1);
	void setRegion(float l, float r, float b, float t);
	void setActive(bool tf);
	void setResolution(int width, int height);

private:
	void createSharedQuad();
	osg::Matrix getCurrWVPById(int id);

	osg::ref_ptr<osg::Camera> _shadowDepthCam;
	osg::ref_ptr<osg::Texture2D> _shadowAtlasTex;

	int _id;
	int _face; // for point light depth cube face
	int _resWidth;
	int _resHeight;

	Light *_light;
	std::vector<osg::ref_ptr<ShadowDepthCameraCallback> >  _updateCallback;

	static osg::ref_ptr<osg::Geode> _quadGeode;
	static int _highest_id;
};

class ShadowDepthCameraCallback : public osg::NodeCallback
{
public:
	ShadowDepthCameraCallback(Light *light, int face=-1);
	virtual void operator()(osg::Node *, osg::NodeVisitor *);

	inline osg::Matrix getCurrWVP()
	{
		return _currWVP;
	}

private:
	void makeLightSpaceMat(osg::Matrix &view, osg::Matrix &proj);
	osg::Matrix calcPointLightViewMat(const osg::Vec3 &pos, int face);
	Light *_light;
	int _face;
	osg::Matrix _currWVP;
};