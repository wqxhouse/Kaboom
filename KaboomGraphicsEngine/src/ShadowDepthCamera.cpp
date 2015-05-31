#include "ShadowDepthCamera.h"
#include <osg/Texture>
#include <osg/Polytope>
#include <math.h> 
#include <osg/ComputeBoundsVisitor>
#include "Core.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "ShadowAtlas.h"
#include "Core.h"
#include "FrustumHelper.h"

ShadowDepthCamera::ShadowDepthCamera(osg::Texture2D *shadowAtlasTex, ShadowAtlas *atlas, osg::Group *geomRoot, Light *light, int face)
	: _shadowAtlasTex(shadowAtlasTex), _atlas(atlas), _light(light), _face(face), _geomRoot(geomRoot)
{
	_id = _highest_id++;

	_shadowDepthCam = new osg::Camera();

	_shadowDepthCam->setClearMask(GL_DEPTH_BUFFER_BIT); 
	_shadowDepthCam->setRenderOrder(osg::Camera::PRE_RENDER, -100);
	_shadowDepthCam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_shadowDepthCam->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	_shadowDepthCam->attach(osg::Camera::DEPTH_BUFFER, _shadowAtlasTex);
	_shadowDepthCam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

	// not sure why, but osg by default generates a color buffer the same size as the attached depth texture.
	// which makes the graphcis memory usage explode... imagine 4096^2 * camNum extra color buffers, easily crashing the graphics driver
	// the following line disables that... Thanks to api-trace, CodeXL debugging tools, and osg source code.
	_shadowDepthCam->setImplicitBufferAttachmentMask(0x0, 0x0);

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
		DirectionalLight *dl = _light->asDirectionalLight();
		ssCoord = dl->getShadowAtlasPos(_face);
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
		DirectionalLight *dl = _light->asDirectionalLight();
		resolution = dl->getShadowMapRes();
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
		if (dl->hasShadowMapAtlasPos(_face))
		{
			return;
		}
		shadowMapIndex = dl->getShadowMapIndex(_face);
		shadowMapRes = dl->getShadowMapRes();
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

	traverse(node, nv);
}

void ShadowDepthCameraCallback::setShadowMapResHelper(Light *light, int resolution)
{
	if (light->getLightType() == DIRECTIONAL)
	{
		DirectionalLight *dl = light->asDirectionalLight();
		dl->setShadowMapRes(resolution);
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
		DirectionalLight *dl = light->asDirectionalLight();
		dl->setShadowAtlasPos(face, atlasPos); // actually face = split, needs refactor them into base class
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
	/*	DirectionalLight *dirLight = _light->asDirectionalLight();
		projMat.makeOrtho(-10.0, 10.0, -10.0, 10.0, 5.0, 1000.0);
		osg::Vec3 wsDir = dirLight->getLightToWorldDirection();
		viewMat.makeLookAt(osg::Vec3(), wsDir, osg::Vec3(0, 0, 1));*/
		DirectionalLight *dl = _light->asDirectionalLight();
		// TODO: revert back to pssm after the demo 
		// makePSSMLightSpaceMat(dl, viewMat, projMat);

		// temporarily use scene bound single shadow map
		makeSceneBoundLightSpaceMat(dl, viewMat, projMat);
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

void ShadowDepthCameraCallback::makeSceneBoundLightSpaceMat(DirectionalLight *dl, osg::Matrix &view, osg::Matrix &proj)
{
	// compute shadowed scene bound
	osg::ComputeBoundsVisitor cv;
	osg::Group *shadowedScene = Core::getWorldRef().getGeometryManager()->getShadowedSceneNode();
	shadowedScene->accept(cv);

	osg::BoundingBox bbox = cv.getBoundingBox();

	// find bary center
	osg::Vec3 center = bbox.center();
	
	// get temp light view matrix
	osg::Vec3 lightToWorld = dl->getLightToWorldDirection();
	lightToWorld.normalize();
	osg::Matrix lightView = osg::Matrix::lookAt(center, lightToWorld, osg::Vec3(0, 0, 1));

	// transform bbox to light space
	osg::Vec3 mins = osg::Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	osg::Vec3 maxes = osg::Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 8; ++i)
	{
		osg::Vec3 lightSpaceCorner = bbox.corner(i) * lightView;
		float minsX = osg::minimum(mins.x(), lightSpaceCorner.x());
		float minsY = osg::minimum(mins.y(), lightSpaceCorner.y());
		float minsZ = osg::minimum(mins.z(), lightSpaceCorner.z());
		mins = osg::Vec3(minsX, minsY, minsZ);

		float maxesX = osg::maximum(maxes.x(), lightSpaceCorner.x());
		float maxesY = osg::maximum(maxes.y(), lightSpaceCorner.y());
		float maxesZ = osg::maximum(maxes.z(), lightSpaceCorner.z());
		maxes = osg::Vec3(maxesX, maxesY, maxesZ);
	}

	osg::Vec3 lightPos = center + (-lightToWorld) * maxes.z();
	
	// get final light viewMat
	osg::Matrix lightViewFinal = osg::Matrix::lookAt(lightPos, center, osg::Vec3(0, 0, 1));

	// get projection matrix
	int filterSize = 3; // hardware 3x3 pcf filter
	float smSize = dl->getShadowMapRes();
	float scale = (smSize + filterSize) / static_cast<float>(smSize);
	mins.x() *= scale;
	mins.y() *= scale;
	maxes.x() *= scale;
	maxes.y() *= scale;

	// osg::Matrix shadowOrthoMat = osg::Matrixd::ortho(mins.x(), maxes.x(), mins.y(), maxes.y(), 0.0, -mins.z());
	// TODO: here hardcoded a scene far bound. (since in the game we do not include the huge sea plane
	// into bound calc to gain higher shadow map pixel intensity. but this means we cannot use 
	// the tight scene bound. or the sea will not correctly shaded).
	osg::Matrix shadowOrthoMat = osg::Matrixd::ortho(mins.x(), maxes.x(), mins.y(), maxes.y(), 0.0, 2000.0);
	view = lightViewFinal;
	proj = shadowOrthoMat;

	//lightPos = -lightToWorld * 50;
	//view = osg::Matrix::lookAt(lightPos, osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));
	//proj = osg::Matrix::ortho(-100, 100, -100, 100, 0, 1000);
}

void ShadowDepthCameraCallback::makePSSMLightSpaceMat(DirectionalLight *dl, osg::Matrix &view, osg::Matrix &proj)
{
	FrustumHelper fdata;
	fdata.updateMatrix(&Core::getMainCamera().getViewMatrix(), &Core::getMainCamera().getProjectionMatrix());
	osg::Vec3 corners[8];

	for (int i = 0; i < 8; i++)
	{
		corners[i] = fdata.corner(i);
	}

	float prevSplitDist = _face == 0 ? 0 : dl->getCascadeSplitDist(_face - 1);
	float splitDist = dl->getCascadeSplitDist(_face);

	// get corners of current cascade
	for (int i = 0; i < 4; i++)
	{
		osg::Vec3 cornerRay = corners[i + 4] - corners[i];
		osg::Vec3 nearCornerRay = cornerRay * prevSplitDist;
		osg::Vec3 farCornerRay = cornerRay * splitDist;
		corners[i + 4] = corners[i] + farCornerRay;
		corners[i] = corners[i] + nearCornerRay;
	}

	// Calculate the centroid of the view frustum slice
	osg::Vec3 ws_viewFrustumCentroid;
	for (int i = 0; i < 8; ++i)
	{
		ws_viewFrustumCentroid = ws_viewFrustumCentroid + corners[i];
	}
	ws_viewFrustumCentroid *= 1.0f / 8.0f;

	// optimize helper
	// osg::Vec3 ws_viewFrustumCentroid = fdata.getCenter();
	osg::Vec3 ws_lightDir = -dl->getLightToWorldDirection();
	ws_lightDir.normalize();

	// use constant z-up for stability.
	//osg::Vec3 upDir = osg::Vec3(0, 0, 1);

	//float bsphereRad = fdata.getBSphereRadius();
	//osg::Vec3 maxExtents = osg::Vec3(bsphereRad, bsphereRad, bsphereRad);
	//osg::Vec3 minExtents = -maxExtents;

	//osg::Vec3 extent = maxExtents - minExtents;
	//osg::Vec3 shadowCamPos = ws_viewFrustumCentroid + ws_lightDir * -minExtents.z();

	//osg::Matrix shadowOrthoMat = osg::Matrixd::ortho(minExtents.x(), maxExtents.x(), minExtents.y(), maxExtents.y(), 0.0, extent.z());
	//osg::Matrix shadowViewMat = osg::Matrixd::lookAt(shadowCamPos, ws_viewFrustumCentroid, upDir);
	//
	//osg::Matrix shadowVPMat = shadowViewMat * shadowOrthoMat;
	//osg::Vec4 origin(0.0f, 0.0f, 0.0f, 1.0f);
	//origin = origin * shadowVPMat;
	//float shadowMapResDiv2 = dl->getShadowMapRes() / 2.0;
	//origin.x() *= shadowMapResDiv2;
	//origin.y() *= shadowMapResDiv2;
	//
	//osg::Vec4 roundedOrigin;
	//roundedOrigin.x() = osg::round(origin.x());
	//roundedOrigin.y() = osg::round(origin.y());
	//roundedOrigin.z() = osg::round(origin.z());
	//roundedOrigin.w() = osg::round(origin.w());
	//
	//osg::Vec4 roundOffset = roundedOrigin - origin;
	//float rcp = 1.0 / shadowMapResDiv2;
	//roundOffset.x() *= rcp;
	//roundOffset.y() *= rcp;
	//roundOffset.z() = 0.0f;
	//roundOffset.w() = 0.0f;
	//
	//shadowOrthoMat(3, 0) += roundOffset.x();
	//shadowOrthoMat(3, 1) += roundOffset.y();


	// Create a temporary view matrix for the light
	osg::Vec3 lookAt = ws_viewFrustumCentroid - ws_lightDir;
	const osg::Matrix &viewMat = Core::getMainCamera().getViewMatrix();

	//osg::Vec3 upDir = osg::Vec3(viewMat(0, 0), viewMat(1, 0), viewMat(2, 0));
	osg::Vec3 upDir = Core::getMainCamera().getUp(); // can we make this assumption that it is always up?
	osg::Matrix lightView = osg::Matrix::lookAt(ws_viewFrustumCentroid, lookAt, upDir);

	// Calculate an AABB around the frustum corners
	osg::Vec3 mins = osg::Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	osg::Vec3 maxes = osg::Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 8; ++i)
	{
		osg::Vec3 lightSpaceCorner = corners[i] * lightView;
		float minsX = osg::minimum(mins.x(), lightSpaceCorner.x());
		float minsY = osg::minimum(mins.y(), lightSpaceCorner.y());
		float minsZ = osg::minimum(mins.z(), lightSpaceCorner.z());
		mins = osg::Vec3(minsX, minsY, minsZ);

		float maxesX = osg::maximum(maxes.x(), lightSpaceCorner.x());
		float maxesY = osg::maximum(maxes.y(), lightSpaceCorner.y());
		float maxesZ = osg::maximum(maxes.z(), lightSpaceCorner.z());
		maxes = osg::Vec3(maxesX, maxesY, maxesZ);

		//mins = osg::minimum(mins, lightSpaceCorner);
		//maxes = osg::maximum(maxes, lightSpaceCorner);
	}

	osg::Vec3 &minExtents = mins;
	osg::Vec3 &maxExtents = maxes;

	// Adjust the min/max to accommodate the filtering size
	int filterSize = 0;
	float smSize = dl->getShadowMapRes();
	float scale = (smSize + filterSize) / static_cast<float>(smSize);
	minExtents.x() *= scale;
	minExtents.y() *= scale;
	maxExtents.x() *= scale;
	maxExtents.y() *= scale;

	osg::Vec3 cascadeExtents = maxExtents - minExtents; 
	osg::Vec3 shadowCamPos = ws_viewFrustumCentroid + ws_lightDir * maxExtents.z();

	osg::Matrix shadowOrthoMat = osg::Matrixd::ortho(minExtents.x(), maxExtents.x(), minExtents.y(), maxExtents.y(), 0.0, cascadeExtents.z());
	osg::Matrix shadowViewMat = osg::Matrixd::lookAt(shadowCamPos, ws_viewFrustumCentroid, upDir);


	// snipping to texel incr
	osg::Matrix vp = shadowViewMat * shadowOrthoMat;
	osg::Vec3 origin(0, 0, 0);
	origin = origin * vp;
	origin *= 0.5;
	origin.x() += 0.5;
	origin.y() += 0.5;
	origin.z() += 0.5;

	double texelSize = 1.0 / dl->getShadowMapRes();
	osg::Vec3 offset;
	offset.x() = std::fmod(offset.x(), texelSize);
	offset.y() = std::fmod(offset.y(), texelSize);
	offset.z() = 0.0;

	osg::Matrix inv_vp = osg::Matrix::inverse(vp);
	osg::Vec3 originNDC = origin - osg::Vec3(offset);
	osg::Vec3 newOrigin = (origin - offset) * 2.0 - osg::Vec3(1.0, 1.0, 1.0);

	shadowCamPos -= newOrigin;
	shadowViewMat = osg::Matrixd::lookAt(shadowCamPos, ws_viewFrustumCentroid, upDir);


	/*mvp = Mat4(source.computeMVP())

		basePoint = mvp.xform(Point4(0, 0, 0, 1))
		texelSize = 1.0 / float(source.resolution)

		basePoint *= 0.5
		basePoint += Vec4(0.5)

		offsetX = basePoint.x % texelSize
		offsetY = basePoint.y % texelSize

		mvp.invertInPlace()
		newBase = mvp.xform(Point4(
		(basePoint.x - offsetX) * 2.0 - 1.0,
		(basePoint.y - offsetY) * 2.0 - 1.0,
		(basePoint.z) * 2.0 - 1.0, 1))
		destPos -= Vec3(newBase.x, newBase.y, newBase.z)*/
	view = shadowViewMat;
	proj = shadowOrthoMat;
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