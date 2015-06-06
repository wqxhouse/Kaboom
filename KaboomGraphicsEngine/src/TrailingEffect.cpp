#include "TrailingEffect.h"
#include <osg/ValueObject>
#include <util/ConfigSettings.h>
#include "ParticleEffect.h"
#include "ParticleEffectManager.h"
#include "Light.h"

#define POINT_PTR "point"

TrailingEffect::TrailingEffect(ParticleEffectManager *manager, SparkUpdatingHandler *handler, float r, float g, float b)
    : ParticleEffect(manager), _handler(handler), r(r), g(g), b(b)
{
	_name = "TrailingEffect";

	ConfigSettings *config = ConfigSettings::config;
	std::string mediaPath;
	config->getValue("MediaPath", mediaPath);
	std::string trailingPath = mediaPath + "DefaultAssets\\ParticleTextures\\Trailing\\";

	// cache texture
	ParticleTexture *particleTex = _manager->getOrCreateParticleTexture(trailingPath + "point.bmp");
	_particleDrawable->addImage(POINT_PTR, particleTex, GL_RGBA);
	_particleDrawable->setBaseSystemCreator(createTrailingEffect);
}

void TrailingEffect::setTrailedObject(GeometryObject *obj)
{
	int sparkIndex = _handler->getSparkIndex(_particleDrawable);
	osg::MatrixTransform *matTrans = obj->getRoot();
    matTrans->setUserValue("r", r);
    matTrans->setUserValue("g", g);
    matTrans->setUserValue("b", b);
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
	int textureParticle = getTextureIdFromTextureIDMap(textureIDMap, POINT_PTR);

	// Renderers
	SPK::GL::GLQuadRenderer* particleRenderer = SPK::GL::GLQuadRenderer::create();
	particleRenderer->enableBlending(true);
	particleRenderer->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE);
	particleRenderer->setTexturingMode(SPK::TEXTURE_2D);
	particleRenderer->setTexture(textureParticle);
	particleRenderer->setTextureBlending(GL_MODULATE);
	particleRenderer->setScale(0.05f, 0.05f);
    particleRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
	particleRenderer->setShared(true);

	SPK::Interpolator* interpolator = NULL; // pointer to an interpolator that is used to retrieve interpolators

	// Model
	SPK::Model* particleModel = SPK::Model::create(SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA | SPK::FLAG_SIZE, SPK::FLAG_ALPHA | SPK::FLAG_SIZE);
	particleModel->setParam(SPK::PARAM_ALPHA, 0.3f, 0.0f);	// the particles will fade as they die
	particleModel->setParam(SPK::PARAM_SIZE, 1.0f, 8.0f);	// the particles will enlarge over time
	particleModel->setLifeTime(0.5f, 1.0f);
	particleModel->setShared(true);

	// Emitter
	// We set up a spheric emitter that emits in all direction with a very small force in order to slightly displace the particles
	SPK::RandomEmitter* particleEmitter = SPK::RandomEmitter::create();
	particleEmitter->setForce(0.01f, 0.01f);
	particleEmitter->setShared(false);

	// Group
	SPK::Group* particleGroup = SPK::Group::create(particleModel, 14000);
	particleGroup->setRenderer(particleRenderer);
	particleGroup->setFriction(-0.3f); // negative friction : The particles will accelerate over time
	particleGroup->addEmitter(particleEmitter);

	// System
	SPK::System* particleSystem = SPK::System::create();
	particleSystem->addGroup(particleGroup);

	return particleSystem->getSPKID();
}
