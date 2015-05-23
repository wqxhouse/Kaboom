#pragma once
#include <osg/Camera>

class ShadowDepthCamera
{
public:
	ShadowDepthCamera(osg::Texture2D *shadowAtlasTex);
	void setRegion(float l, float r, float b, float t);
	void setActive(bool tf);

private:
	void createSharedQuad();

	osg::ref_ptr<osg::Camera> _shadowDepthCam;
	osg::ref_ptr<osg::Texture2D> _shadowAtlasTex;
	static osg::ref_ptr<osg::Geode> _quadGeode;
};