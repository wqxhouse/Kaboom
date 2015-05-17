#include "TrailingEffect.h"
#include <util/ConfigSettings.h>
#include "ParticleEffect.h"
#include "ParticleEffectManager.h"
#include "Light.h"

#define SMOKE_STR "smoke"

TrailingEffect::TrailingEffect(ParticleEffectManager *manager, SparkUpdatingHandler *handler)
	: ParticleEffect(manager), _handler(handler)
{
	_name = "TrailingEffect";

	ConfigSettings *config = ConfigSettings::config;
	std::string mediaPath;
	config->getValue("MediaPath", mediaPath);
	std::string trailingPath = mediaPath + "DefaultAssets\\ParticleTextures\\Trailing\\";

	// cache texture
	ParticleTexture *smokeTex = _manager->getOrCreateParticleTexture(trailingPath + "smoke.png");
	_particleDrawable->addImage(SMOKE_STR, smokeTex, GL_RGBA);
	_particleDrawable->setBaseSystemCreator(createTrailingEffect);
}

void TrailingEffect::setTrailedObject(GeometryObject *obj)
{
	int sparkIndex = _handler->getSparkIndex(_particleDrawable);
	osg::MatrixTransform *matTrans = obj->getRoot();
	_handler->setTrackee(sparkIndex, matTrans);
}

void TrailingEffect::setTrailedObject(Light *light)
{
	// TODO: impl
}

SPK::SPK_ID TrailingEffect::createTrailingEffect(
	const SparkDrawable::TextureIDMap& textureIDMap,
	int screenWidth, int screenHeight)
{
	int textureParticle = getTextureIdFromTextureIDMap(textureIDMap, SMOKE_STR);
	SPK::GL::GLQuadRenderer* particleRenderer = SPK::GL::GLQuadRenderer::create();
	particleRenderer->setTexturingMode(SPK::TEXTURE_2D);
	particleRenderer->setAtlasDimensions(2, 2);
	particleRenderer->setTexture(textureParticle);
	particleRenderer->setTextureBlending(GL_MODULATE);
	particleRenderer->setScale(0.05f, 0.05f);
	particleRenderer->setBlending(SPK::BLENDING_ADD);
	particleRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);

	// Model
	SPK::Model* particleModel = SPK::Model::create(
		SPK::FLAG_SIZE | SPK::FLAG_ALPHA | SPK::FLAG_TEXTURE_INDEX | SPK::FLAG_ANGLE,
		SPK::FLAG_SIZE | SPK::FLAG_ALPHA,
		SPK::FLAG_SIZE | SPK::FLAG_TEXTURE_INDEX | SPK::FLAG_ANGLE);
	particleModel->setParam(SPK::PARAM_SIZE, 0.5f, 1.0f, 10.0f, 20.0f);
	particleModel->setParam(SPK::PARAM_ALPHA, 1.0f, 0.0f);
	particleModel->setParam(SPK::PARAM_ANGLE, 0.0f, 2.0f * osg::PI);
	particleModel->setParam(SPK::PARAM_TEXTURE_INDEX, 0.0f, 4.0f);
	particleModel->setLifeTime(2.0f, 5.0f);

	// Emitter
	SPK::SphericEmitter* particleEmitter = SPK::SphericEmitter::create(
		SPK::Vector3D(-1.0f, 0.0f, 0.0f), 0.0f, 0.1f * osg::PI);
	particleEmitter->setZone(SPK::Point::create(SPK::Vector3D(0.0f, 0.015f, 0.0f)));
	particleEmitter->setFlow(250.0);
	particleEmitter->setForce(1.5f, 1.5f);

	// Group
	SPK::Group* particleGroup = SPK::Group::create(particleModel, 500);
	particleGroup->addEmitter(particleEmitter);
	particleGroup->setRenderer(particleRenderer);
	particleGroup->setGravity(SPK::Vector3D(0.0f, 0.0f, 0.05f));
	particleGroup->enableAABBComputing(true);

	SPK::System* particleSystem = SPK::System::create();
	particleSystem->addGroup(particleGroup);
	particleSystem->enableAABBComputing(true);
	return particleSystem->getSPKID();
}