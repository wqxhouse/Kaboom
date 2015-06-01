#include "ShadowManager.h"

#include <cmath>
#include <osg/TextureCubeMap>
#include <osgDB/ReadFile>

#include "EffectCompositor.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShadowDepthCamera.h"
#include "LightManager.h"

#include "Core.h"

#define MAX_SHADOW_MAPS 60
#define SHADOW_ATLAS_RESOLUTION 4096

ShadowManager::ShadowManager(osgFX::EffectCompositor *passes, osg::Group *geomRoot)
	: _passes(passes), _geomRoot(geomRoot), _currShadowMapNum(0)
{
	_atlas = new ShadowAtlas();
	_atlas->createAtlas(SHADOW_ATLAS_RESOLUTION);

	_depthCamGroup = new osg::Group;
	_depthCamGroup->setUpdateCallback(new DepthCamGroupCallback);
	// disable all shaders when rendering depth
	_depthCamGroup->getOrCreateStateSet()->setAttribute(new osg::Program(), osg::StateAttribute::OVERRIDE);

	_depthCameras.resize(MAX_SHADOW_MAPS);
	for (int i = 0; i < MAX_SHADOW_MAPS; i++)
	{
		_depthCameras[i] = NULL;
	}

	getPassInfo();

	_pointLightShadowFaceLookupTex = new osg::TextureCubeMap;
	configShadowMapFaceLookupCube();

	//static osg::ref_ptr<osg::Camera> _abcd = new osg::Camera;
	//_abcd->setClearDepth(1.0);
	//_abcd->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
	//_abcd->setClearMask(GL_DEPTH_BUFFER_BIT);
	//_abcd->attach(osg::Camera::DEPTH_BUFFER, _depthAtlasTex.get());
	//osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	//depth->setFunction(osg::Depth::ALWAYS);
	//depth->setWriteMask(false);
	//_abcd->getOrCreateStateSet()->setAttribute(depth);
	//_abcd->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	//_abcd->setRenderOrder(osg::Camera::PRE_RENDER, -101);
	//_abcd->setImplicitBufferAttachmentMask(0x0, 0x0);
	//_depthCamGroup->addChild(_abcd);
}

ShadowManager::~ShadowManager()
{
	delete _atlas;
	for (int i = 0; i < MAX_SHADOW_MAPS; i++)
	{
		if (_depthCameras[i] != NULL)
		{
			delete _depthCameras[i];
		}
	}
}

void ShadowManager::configShadowMapFaceLookupCube()
{
	osg::TextureCubeMap *cubemap = _pointLightShadowFaceLookupTex;
	std::string texPath = Core::getMediaPath() + "DefaultAssets\\PointLightShadowLookup\\";

	for (int i = 0; i < 6; i++)
	{
		osg::Image *img = osgDB::readImageFile(texPath + std::to_string(i) + ".png");
		if (img == NULL)
		{
			OSG_WARN << "ShadowManager: cubemap tex not found" << std::endl;
			continue;
		}
		cubemap->setImage(osg::TextureCubeMap::POSITIVE_X + i, img);
	}

	cubemap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	cubemap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
	cubemap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);
	cubemap->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
	cubemap->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
	cubemap->setResizeNonPowerOfTwoHint(false);
}

bool ShadowManager::hasCameraAtIndex(int index)
{
	return _depthCameras[index] != NULL ? true : false;
}

int ShadowManager::getNumShadowMaps()
{
	return MAX_SHADOW_MAPS;
}

void ShadowManager::getPassInfo()
{
	_depthAtlasTex = static_cast<osg::Texture2D *>(_passes->getTexture("b_shadowAtlas"));
}

void ShadowManager::addDirectionalLight(DirectionalLight *light)
{
	int resolution = light->getShadowMapRes();
	int tileSize = _atlas->getTileSize();
	int atlasSize = _atlas->getSize();
	if (resolution < tileSize || resolution % tileSize != 0)
	{
		OSG_WARN << "The cubemap shadow resolution has to be a multiple of " << tileSize << std::endl;
		light->setShadowMapRes(tileSize);
		resolution = tileSize;
	}

	if (resolution > atlasSize)
	{
		OSG_WARN << "The cubemap shadow resolution cannot be larger than " << atlasSize << std::endl;
		light->setShadowMapRes(tileSize);
		resolution = tileSize;
	}

	for (int i = 0; i < light->getNumSplits(); i++)
	{
		// FIXME: same problem as pointlight;  in addPointLight()
		int slot = findAvailableDepthSlot();
		if (slot == -1)
		{
			OSG_WARN << "no available shadow map slots" << std::endl;
			return;
		}

		ShadowDepthCamera *depthCam = new ShadowDepthCamera(_depthAtlasTex.get(), _atlas, _geomRoot.get(), light, i);
		_depthCameras[_currShadowMapNum] = depthCam;
		_depthCamGroup->addChild(depthCam->getRoot());

		light->setShadowMapIndex(i, slot);
		_currShadowMapNum++;
	}
}

void ShadowManager::addPointLight(PointLight *light)
{
	int resolution = light->getShadowMapRes();
	int tileSize = _atlas->getTileSize();
	int atlasSize = _atlas->getSize();
	if (resolution < tileSize || resolution % tileSize != 0)
	{
		OSG_WARN << "The cubemap shadow resolution has to be a multiple of " << tileSize << std::endl;
		light->setShadowMapRes(tileSize);
		resolution = tileSize;
	}

	if (resolution > atlasSize)
	{
		OSG_WARN << "The cubemap shadow resolution cannot be larger than " << atlasSize << std::endl;
		light->setShadowMapRes(tileSize);
		resolution = tileSize;
	}

	// six faces
	for (int i = 0; i < 6; i++)
	{
		// FIXME: this will possibly make cubemap faces incomplete in the atlas
		// can skip all 6 faces if < 6
		int slot = findAvailableDepthSlot();
		if (slot == -1)
		{
			OSG_WARN << "no available shadow map slots" << std::endl;
			return;
		}

		ShadowDepthCamera *depthCam = new ShadowDepthCamera(_depthAtlasTex.get(), _atlas, _geomRoot.get(), light, i);
		_depthCameras[_currShadowMapNum] = depthCam;
		_depthCamGroup->addChild(depthCam->getRoot());

		light->setShadowMapIndex(i, slot);
		_currShadowMapNum++;
	}
}

void ShadowManager::removePointLight(PointLight *light)
{
	if (light->getCastShadow())
	{
		for (int i = 0; i < 6; i++)
		{
			int index = light->getShadowMapIndex(i);
			ShadowDepthCamera *depthCamera = _depthCameras[index];
			osg::Camera *cam = depthCamera->getRoot();
			int cc = cam->getThreadSafeReferenceCounting();
			_depthCamGroup->removeChild(cam);
			int dd = cam->getThreadSafeReferenceCounting();
			
			delete depthCamera;
			int ee = cam->getThreadSafeReferenceCounting();
			_depthCameras[index] = NULL;
			_atlas->removeTile(index);
		}
	}
}

int ShadowManager::findAvailableDepthSlot()
{
	for (int i = 0; i < _depthCameras.size(); i++)
	{
		if (_depthCameras[i] == NULL)
		{
			return i;
		}
	}
	
	return -1;
}

const osg::Matrix &ShadowManager::getLightSpaceWVP(int index)
{
	if (_depthCameras[index] == NULL) return _defaultMatrix;
	return _depthCameras[index]->getCurrWVP();
}

float ShadowManager::getShadowMapScaleWRTAtlas(int index)
{
	if (_depthCameras[index] == NULL) return -1.0f;
	return _depthCameras[index]->getShadowMapScaleWRTAtlas();
}

osg::Vec2 ShadowManager::getAtlasPosUVCoord(int index)
{
	if (_depthCameras[index] == NULL) return osg::Vec2(-1, -1);
	return _depthCameras[index]->getAtlasPosUVCoord();
}

//=====================================================================
DepthCamGroupCallback::DepthCamGroupCallback()
{
	// TODO: hacky
	_lmanager = Core::getWorldRef().getLightManager();
}

void DepthCamGroupCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	if (nv->getFrameStamp()->getFrameNumber() == 1) return;
	//for (int i = 0; i < _lmanager->getNumLights(); i++)
	//{
	//	Light *l = _lmanager->getLight(i);
	//	if (l->getLightType() == DIRECTIONAL)
	//	{
	//		DirectionalLight *dl = l->asDirectionalLight();
	//		Camera &camera = Core::getMainCamera();
	//		float nearClip = camera.getNearPlane();
	//		float farClip = camera.getFarPlane();
	//		float clipRange = farClip - nearClip;

	//		float minZ = nearClip;
	//		float shadowFar = dl->getShadowFarPlane();
	//		float maxZ = nearClip + ((shadowFar > farClip ? farClip : shadowFar) / farClip) * clipRange;

	//		float range = maxZ - minZ;
	//		float ratio = maxZ / minZ;

	//		int numSplits = dl->getNumSplits();
	//		for (int j = 0; j < numSplits; j++)
	//		{
	//			float p = (j + 1) / static_cast<float>(numSplits);
	//			float log = minZ * std::pow(ratio, p);
	//			float uniform = minZ + range * p;
	//			float d = (log - uniform) + uniform;
	//			float dist = (d - nearClip) / clipRange;
	//			dl->setCascadeSplitDist(j, dist);
	//		}
	//	}

	//}
	traverse(node, nv);
}
