#include "ShadowManager.h"
#include "EffectCompositor.h"

ShadowManager::ShadowManager(osgFX::EffectCompositor *passes, osg::Group *geomRoot)
	: _passes(passes), _geomRoot(geomRoot)
{
	_atlas = new ShadowAtlas();
	_atlas->setSize(4096);
	_atlas->createAtlas();


	// getPassCam();
}

ShadowManager::~ShadowManager()
{
	delete _atlas;
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

void ShadowManager::addPointLight(PointLight *light)
{
	// todo
}