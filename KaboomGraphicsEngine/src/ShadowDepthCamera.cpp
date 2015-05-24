#include "ShadowDepthCamera.h"
#include <osg/Texture>
#include "PointLight.h"
#include "DirectionalLight.h"

ShadowDepthCamera::ShadowDepthCamera(osg::Texture2D *shadowAtlasTex, Light *light, int face)
	: _shadowAtlasTex(shadowAtlasTex), _light(light), _face(face), _resWidth(512), _resHeight(512)
{
	if (_quadGeode == NULL)
	{
		createSharedQuad();
	}

	_id = _highest_id++;

	_shadowDepthCam = new osg::Camera();
	// _shadowDepthCam->addChild(_quadGeode);

	_shadowDepthCam->setClearMask(GL_DEPTH_BUFFER_BIT); // do not clear color buffer, or atlas will be destroyed
	_shadowDepthCam->setRenderOrder(osg::Camera::PRE_RENDER);
	_shadowDepthCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_shadowDepthCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_shadowDepthCam->attach(osg::Camera::COLOR_BUFFER, _shadowAtlasTex);

	ShadowDepthCameraCallback *callback = new ShadowDepthCameraCallback(_light, face);
	_updateCallback.push_back(callback);
	_shadowDepthCam->setUpdateCallback(callback);
}

void ShadowDepthCamera::setResolution(int width, int height)
{
	_resWidth = width;
	_resHeight = height;
	// TODO: update camera ? 
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

osg::Matrix ShadowDepthCamera::getCurrWVPById(int id)
{
	ShadowDepthCameraCallback *callback = _updateCallback[id];
	return callback->getCurrWVP();
}

// === === === === === === === === === === ===  
ShadowDepthCameraCallback::ShadowDepthCameraCallback(Light *light, int face)
	: _light(light), _face(face)
{
}

void ShadowDepthCameraCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
	osg::Camera *cam = static_cast<osg::Camera *>(node);
	osg::Matrix viewMat;
	osg::Matrix projMat;
	makeLightSpaceMat(viewMat, projMat);
	cam->setViewMatrix(viewMat);
	cam->setProjectionMatrix(projMat);
}

void ShadowDepthCameraCallback::makeLightSpaceMat(osg::Matrix &view, osg::Matrix &proj)
{
	osg::Matrix projMat;
	osg::Matrix viewMat;
	if (_light->getLightType() == DIRECTIONAL)
	{
		DirectionalLight *dirLight = _light->asDirectionalLight();
		projMat.makeOrtho(-10.0, 10.0, -10.0, 10.0, 5.0, 1000.0);
		osg::Vec3 wsDir = dirLight->getLightToWorldDirection();
		viewMat.makeLookAt(osg::Vec3(), wsDir, osg::Vec3(0, 0, 1));
	}
	else if (_light->getLightType() == POINTLIGHT)
	{
		PointLight *pt = _light->asPointLight();
		projMat.makePerspective(60, 1.0, 1.0, pt->getRadius());
		viewMat = calcPointLightViewMat(pt->getPosition(), _face);
	}

	_currWVP = viewMat * projMat;
	view = viewMat;
	proj = projMat;
}

osg::Matrix ShadowDepthCameraCallback::calcPointLightViewMat(const osg::Vec3 &pos, int face)
{
	// std::cout << face << std::endl;
	static const osg::Vec3 viewDirs[6] = {
		osg::Vec3(1, 0, 0),
		osg::Vec3(-1, 0, 0),
		osg::Vec3(0, 1, 0),
		osg::Vec3(0, -1, 0),
		osg::Vec3(0, 0, 1),
		osg::Vec3(0, 0, -1)
	};

	static const osg::Vec3 upDirs[6] = {
		osg::Vec3(0, -1, 0),
		osg::Vec3(0, -1, 0),
		osg::Vec3(0, 0, 1),
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, -1, 0),
		osg::Vec3(0, -1, 0),
	};

	if (face < 0 || face > 5)
	{
		return osg::Matrix();
	}

	osg::Matrix cam;
	cam.makeLookAt(pos, pos + viewDirs[face], upDirs[face]);

	return cam;
}

osg::ref_ptr<osg::Geode> ShadowDepthCamera::_quadGeode = NULL;
int ShadowDepthCamera::_highest_id = 0;