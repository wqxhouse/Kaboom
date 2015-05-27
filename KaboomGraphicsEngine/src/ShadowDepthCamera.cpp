#include "ShadowDepthCamera.h"
#include <osg/Texture>
#include "PointLight.h"
#include "DirectionalLight.h"
#include "ShadowAtlas.h"

ShadowDepthCamera::ShadowDepthCamera(osg::Texture2D *shadowAtlasTex, ShadowAtlas *atlas, osg::Group *geomRoot, Light *light, int face)
	: _shadowAtlasTex(shadowAtlasTex), _atlas(atlas), _light(light), _face(face), _geomRoot(geomRoot)
{
	_id = _highest_id++;

	_shadowDepthCam = new osg::Camera();

	_shadowDepthCam->setClearMask(GL_DEPTH_BUFFER_BIT); // do not clear color buffer, or atlas will be destroyed
	_shadowDepthCam->setRenderOrder(osg::Camera::PRE_RENDER);
	_shadowDepthCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_shadowDepthCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_shadowDepthCam->attach(osg::Camera::DEPTH_BUFFER, _shadowAtlasTex);
	_shadowDepthCam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

	ShadowDepthCameraCallback *callback = new ShadowDepthCameraCallback(_atlas, _light, face);
	_updateCallback = callback;
	_shadowDepthCam->setUpdateCallback(callback);
	_shadowDepthCam->addChild(_geomRoot.get());
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


osg::Vec2 ShadowDepthCamera::getAtlasPosUVCoord()
{
	auto type = _light->getLightType();
	osg::Vec2i ssCoord = osg::Vec2i();
	if (type == DIRECTIONAL)
	{
		// todo	
	}
	else if (type == POINTLIGHT)
	{
		PointLight *pl = _light->asPointLight();
		ssCoord = pl->getShadowAtlasPos(_face);
	}

	int atlasSize = _atlas->getSize();
	double x = (double)ssCoord.x() / atlasSize;
	double y = (double)ssCoord.y() / atlasSize;
	return osg::Vec2(x, y);
}

float ShadowDepthCamera::getShadowMapScaleWRTAtlas()
{
	auto type = _light->getLightType();
	int resolution = 0;
	if (type == DIRECTIONAL)
	{
		// todo	
	}
	else if (type == POINTLIGHT)
	{
		PointLight *pl = _light->asPointLight();
		resolution = pl->getShadowMapRes();
	}

	double texScale = (double)resolution / _atlas->getSize();
	return texScale;
}

// todo, move this to manager
const osg::Matrix &ShadowDepthCamera::getCurrWVP()
{
	return _updateCallback->getCurrWVP();
}

// === === === === === === === === === === ===  
ShadowDepthCameraCallback::ShadowDepthCameraCallback(ShadowAtlas *atlas, Light *light, int face)
	: _atlas(atlas), _light(light), _face(face)
{
}

void ShadowDepthCameraCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
	// todo: check need update

	// calc mvp
	osg::Camera *cam = static_cast<osg::Camera *>(node);
	osg::Matrix viewMat;
	osg::Matrix projMat;
	makeLightSpaceMat(viewMat, projMat);
	cam->setViewMatrix(viewMat);
	cam->setProjectionMatrix(projMat);

	// Determine render viewport
	int shadowMapIndex = -1;
	int shadowMapRes = -1;
	if (_light->getLightType() == DIRECTIONAL)
	{
		DirectionalLight *dl = _light->asDirectionalLight();
		// todo
	}
	else if (_light->getLightType() == POINTLIGHT)
	{
		PointLight *pl = _light->asPointLight();
		if (pl->hasShadowMapAtlasPos(_face))
		{
			return;
		}
		shadowMapIndex = pl->getShadowMapIndex(_face);
		shadowMapRes = pl->getShadowMapRes();
	}

	// todo: make sure using shadowmap index doesn't not produce conflicts or override
	osg::Vec2i atlasPos = _atlas->createTile(shadowMapIndex, shadowMapRes, shadowMapRes);
	if (atlasPos.x() < 0)
	{
		int tileSize = _atlas->getTileSize();
		setShadowMapResHelper(_light, tileSize);
		shadowMapRes = tileSize;
		
		// try again
		atlasPos = _atlas->createTile(shadowMapIndex,
			tileSize, tileSize);
		if (atlasPos.x() < 0)
		{
			OSG_WARN << "ShadowDepthCamera:: Shadow atlas full. "
				"Atlas resolution dynamic change to be implemented..." << std::endl;
		}
	}

	setShadowMapAtlasPosHelper(_light, atlasPos, _face);
	cam->setViewport(atlasPos.x(), atlasPos.y(), shadowMapRes, shadowMapRes);
}

void ShadowDepthCameraCallback::setShadowMapResHelper(Light *light, int resolution)
{
	if (light->getLightType() == DIRECTIONAL)
	{

	}
	else if (light->getLightType() == POINTLIGHT)
	{
		PointLight *pt = light->asPointLight();
		pt->setShadowMapRes(resolution);
	}
}

void ShadowDepthCameraCallback::setShadowMapAtlasPosHelper(Light *light, const osg::Vec2i &atlasPos, int face)
{
	if (light->getLightType() == DIRECTIONAL)
	{

	}
	else if (light->getLightType() == POINTLIGHT)
	{
		PointLight *pt = light->asPointLight();
		pt->setShadowAtlasPos(face, atlasPos);
	}
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
		projMat.makePerspective(95, 1.0, 0.5, pt->getRadius());
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
		osg::Vec3(0, 0, 1),
		osg::Vec3(0, 0, 1),
		osg::Vec3(0, 0, 1),
		osg::Vec3(0, 0, 1),
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

int ShadowDepthCamera::_highest_id = 0;