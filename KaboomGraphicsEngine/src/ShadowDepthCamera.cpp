#include "ShadowDepthCamera.h"
#include <osg/Texture>

ShadowDepthCamera::ShadowDepthCamera(osg::Texture2D *shadowAtlasTex)
	: _shadowAtlasTex(shadowAtlasTex)
{
	if (_quadGeode == NULL)
	{
		createSharedQuad();
	}

	_shadowDepthCam = new osg::Camera();
	_shadowDepthCam->addChild(_quadGeode);

	//TODO: projMat
	//osg::Matrix projMat;
	//projMat.makePerspective(90, 1, 1, 1000);
	//_shadowDepthCam->setProjectionMatrix(projMat);

	_shadowDepthCam->setClearMask(GL_DEPTH_BUFFER_BIT); // do not clear color buffer, or atlas will be destroyed
	_shadowDepthCam->setRenderOrder(osg::Camera::PRE_RENDER);
	_shadowDepthCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_shadowDepthCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_shadowDepthCam->attach(osg::Camera::COLOR_BUFFER, _shadowAtlasTex);
}

void ShadowDepthCamera::createSharedQuad()
{
	_quadGeode = new osg::Geode;
	osg::Drawable *d =
		osg::createTexturedQuadGeometry(
		osg::Vec3(), osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 1.0f, 0.0f));
	_quadGeode->addDrawable(d);
}

void ShadowDepthCamera::setRegion(float l, float r, float b, float t)
{
	_shadowDepthCam->setViewport(l, r, b, t);
}

void ShadowDepthCamera::setActive(bool tf)
{
	if (tf)
	{
		_shadowDepthCam->setNodeMask(~0x0);
	}
	else
	{
		_shadowDepthCam->setNodeMask(0x0);
	}
}

static osg::ref_ptr<osg::Geode> _quadGeode = NULL;