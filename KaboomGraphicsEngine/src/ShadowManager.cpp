#include "ShadowManager.h"
#include "EffectCompositor.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ShadowDepthCamera.h"

#define MAX_SHADOW_MAPS 60
#define SHADOW_ATLAS_RESOLUTION 4096
ShadowManager::ShadowManager(osgFX::EffectCompositor *passes, osg::Group *geomRoot)
	: _passes(passes), _geomRoot(geomRoot)
{
	_atlas = new ShadowAtlas();
	_atlas->createAtlas(SHADOW_ATLAS_RESOLUTION);

	_depthCameras.resize(MAX_SHADOW_MAPS);
	for (int i = 0; i < MAX_SHADOW_MAPS; i++)
	{
		_depthCameras[i] = NULL;
	}

	// getPassCam();
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

void ShadowManager::getPassInfo()
{
	osgFX::EffectCompositor::PassData depthPassData;
	_passes->getPassData("ShadowDepthPass", depthPassData);
	_shadowDepthPassCam = depthPassData.pass;

	_depthAtlasTex = static_cast<osg::Texture2D *>(_passes->getTexture("b_shadowAtlas"));
}

void ShadowManager::addDirectionalLight(DirectionalLight *light)
{
	// todo
}

int ShadowManager::findAvailableDepthSlot()
{
	for (int i = 0; i < _depthCameras.size(); i++)
	{
		if (_depthCameras[i] != NULL)
		{
			return i;
		}
	}
	
	return -1;
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

		ShadowDepthCamera *depthCam = new ShadowDepthCamera(_depthAtlasTex.get(), _atlas,light, i);
		// depthCam->setResolution(resolution, resolution);
		light->setShadowMapIndex(i, slot);
	}
}