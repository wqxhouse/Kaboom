#include "ExplosionEffect2.h"
#include <util/ConfigSettings.h>

#include "ParticleEffectManager.h"

// find a better way
#define EXPLOSION_STR "explosion"
#define FLASH_STR "flash"
#define SPARK1_STR "spark1"
#define SPARK2_STR "spark2"
#define WAVE_STR "wave"

ExplosionEffect2::ExplosionEffect2(ParticleEffectManager *manager)
    : ParticleEffect(manager) {
    _name = "Explosion";

    ConfigSettings *config = ConfigSettings::config;
    std::string mediaPath;
    config->getValue("MediaPath", mediaPath);
    std::string explosionPath = mediaPath + "DefaultAssets\\ParticleTextures\\Explosion\\";
    std::string explosion2Path = mediaPath + "DefaultAssets\\ParticleTextures\\Explosion2\\";

    // cache texture
    ParticleTexture *expTex = _manager->getOrCreateParticleTexture(explosion2Path + "flame.png");
    ParticleTexture *flashTex = _manager->getOrCreateParticleTexture(explosion2Path + "flash.png");
    ParticleTexture *spark1Tex = _manager->getOrCreateParticleTexture(explosion2Path + "spark.png");
    ParticleTexture *spark2Tex = _manager->getOrCreateParticleTexture(explosionPath + "point.bmp");
    ParticleTexture *waveTex = _manager->getOrCreateParticleTexture(explosion2Path + "shockwave.png");

    _particleDrawable->addImage(EXPLOSION_STR, expTex, GL_ALPHA);
    _particleDrawable->addImage(FLASH_STR, flashTex, GL_ALPHA);
    _particleDrawable->addImage(SPARK1_STR, spark1Tex, GL_ALPHA);
    _particleDrawable->addImage(SPARK2_STR, spark2Tex, GL_ALPHA);
    _particleDrawable->addImage(WAVE_STR, waveTex, GL_ALPHA);

    _particleDrawable->setSortParticles(true);
    _particleDrawable->setBaseSystemCreator(createExplosionEffect2);
}

//void ExplosionEffect2::setExplosionRadius(float radius)
//{ 
//
//}

SPK::SPK_ID ExplosionEffect2::createExplosionEffect2(
    const SparkDrawable::TextureIDMap& textureIDMap,
    int screenWidth, int screenHeight) {
    int textureExplosion = getTextureIdFromTextureIDMap(textureIDMap, EXPLOSION_STR);
    int textureFlash = getTextureIdFromTextureIDMap(textureIDMap, FLASH_STR);
    int textureSpark1 = getTextureIdFromTextureIDMap(textureIDMap, SPARK1_STR);
    int textureSpark2 = getTextureIdFromTextureIDMap(textureIDMap, SPARK2_STR);
    int textureWave = getTextureIdFromTextureIDMap(textureIDMap, WAVE_STR);

    //
    // Renderers
    //

    // smoke renderer
    SPK::GL::GLQuadRenderer* smokeRenderer = SPK::GL::GLQuadRenderer::create();
    smokeRenderer->setTexturingMode(SPK::TEXTURE_2D);
    smokeRenderer->setTexture(textureExplosion);
    smokeRenderer->setTextureBlending(GL_MODULATE);
    smokeRenderer->setAtlasDimensions(2, 2); // uses 4 different patterns in the texture
    smokeRenderer->setBlending(SPK::BLENDING_ALPHA);
    smokeRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
    smokeRenderer->setShared(true);

    // flame renderer
    SPK::GL::GLQuadRenderer* flameRenderer = SPK::GL::GLQuadRenderer::create();
    flameRenderer->setTexturingMode(SPK::TEXTURE_2D);
    flameRenderer->setTexture(textureExplosion);
    flameRenderer->setTextureBlending(GL_MODULATE);
    flameRenderer->setAtlasDimensions(2, 2);
    flameRenderer->setBlending(SPK::BLENDING_ALPHA);
    flameRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
    flameRenderer->setShared(true);

    // flash renderer
    SPK::GL::GLQuadRenderer* flashRenderer = SPK::GL::GLQuadRenderer::create();
    flashRenderer->setTexturingMode(SPK::TEXTURE_2D);
    flashRenderer->setTexture(textureFlash);
    flashRenderer->setTextureBlending(GL_MODULATE);
    flameRenderer->setAtlasDimensions(2, 2);
    flashRenderer->setBlending(SPK::BLENDING_ALPHA);
    flashRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
    flashRenderer->setShared(true);

    // spark 1 renderer
    SPK::GL::GLQuadRenderer* spark1Renderer = SPK::GL::GLQuadRenderer::create();
    spark1Renderer->setTexturingMode(SPK::TEXTURE_2D);
    spark1Renderer->setTexture(textureSpark1);
    spark1Renderer->setTextureBlending(GL_MODULATE);
    spark1Renderer->setBlending(SPK::BLENDING_ALPHA);
    spark1Renderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
    spark1Renderer->setOrientation(SPK::DIRECTION_ALIGNED); // sparks are oriented function o their velocity
    spark1Renderer->setScale(0.05f, 1.0f); // thin rectangles
    spark1Renderer->setShared(true);

    // spark 2 renderer
    SPK::GL::GLRenderer* spark2Renderer = NULL;
    if (SPK::GL::GLPointRenderer::loadGLExtPointSprite() &&
        SPK::GL::GLPointRenderer::loadGLExtPointParameter()) // uses point sprite if possible
    {
        SPK::GL::GLPointRenderer::setPixelPerUnit(45.0f * osg::PI / 180.0f, screenHeight);
        SPK::GL::GLPointRenderer* pointRenderer = SPK::GL::GLPointRenderer::create();
        pointRenderer->setType(SPK::POINT_SPRITE);
        pointRenderer->setTexture(textureSpark2);
        pointRenderer->setTextureBlending(GL_MODULATE);
        pointRenderer->enableWorldSize(true);
        pointRenderer->setSize(0.02f);
        spark2Renderer = pointRenderer;
    } else {
        SPK::GL::GLQuadRenderer* quadRenderer = SPK::GL::GLQuadRenderer::create();
        quadRenderer->setTexturingMode(SPK::TEXTURE_2D);
        quadRenderer->setTexture(textureSpark2);
        quadRenderer->setTextureBlending(GL_MODULATE);
        quadRenderer->setScale(0.02f, 0.02f);
        spark2Renderer = quadRenderer;
    }
    spark2Renderer->setBlending(SPK::BLENDING_ADD);
    spark2Renderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
    spark2Renderer->setShared(true);

    // wave renderer
    SPK::GL::GLQuadRenderer* waveRenderer = SPK::GL::GLQuadRenderer::create();
    waveRenderer->setTexturingMode(SPK::TEXTURE_2D);
    waveRenderer->setTexture(textureWave);
    waveRenderer->setTextureBlending(GL_MODULATE);
    waveRenderer->setBlending(SPK::BLENDING_ALPHA);
    waveRenderer->enableRenderingHint(SPK::DEPTH_WRITE, false);
    waveRenderer->enableRenderingHint(SPK::ALPHA_TEST, true); // uses the alpha test
    waveRenderer->setAlphaTestThreshold(0.0f);
    waveRenderer->setOrientation(SPK::FIXED_ORIENTATION); // the orientatin is fixed
    waveRenderer->lookVector.set(0.0f, 0.0f, 1.0f);
    waveRenderer->upVector.set(1.0f, 0.0f, 0.0f); // we dont really care about the up axis
    waveRenderer->setShared(true);

    //
    // Models
    //
    SPK::Interpolator* interpolator = NULL; // pointer to an interpolator that is used to retrieve interpolators    

    // smoke model
    SPK::Model* smokeModel = SPK::Model::create(
        SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA |
        SPK::FLAG_SIZE | SPK::FLAG_ANGLE | SPK::FLAG_TEXTURE_INDEX,
        SPK::FLAG_SIZE | SPK::FLAG_ANGLE,
        SPK::FLAG_SIZE | SPK::FLAG_ANGLE | SPK::FLAG_TEXTURE_INDEX,
        SPK::FLAG_ALPHA);
    smokeModel->setParam(SPK::PARAM_RED, 0.7f);
    smokeModel->setParam(SPK::PARAM_GREEN, 1.0f);
    smokeModel->setParam(SPK::PARAM_BLUE, 1.0f);
    smokeModel->setParam(SPK::PARAM_SIZE, 0.6f, 0.8f, 1.0f, 1.4f);
    smokeModel->setParam(SPK::PARAM_TEXTURE_INDEX, 0.0f, 4.0f);
    smokeModel->setParam(SPK::PARAM_ANGLE, 0.0f, osg::PI * 0.5f, 0.0f, osg::PI * 0.5f);
    smokeModel->setLifeTime(2.5f, 3.0f);
    smokeModel->setShared(true);

    interpolator = smokeModel->getInterpolator(SPK::PARAM_ALPHA);
    interpolator->addEntry(0.0f, 0.0f);
    interpolator->addEntry(0.4f, 0.4f, 0.6f);
    interpolator->addEntry(0.6f, 0.4f, 0.6f);
    interpolator->addEntry(1.0f, 0.0f);

    // flame model
    SPK::Model* flameModel = SPK::Model::create(
        SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA |
        SPK::FLAG_SIZE | SPK::FLAG_ANGLE | SPK::FLAG_TEXTURE_INDEX,
        SPK::FLAG_ANGLE | SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE,
        SPK::FLAG_ANGLE | SPK::FLAG_TEXTURE_INDEX,
        SPK::FLAG_SIZE | SPK::FLAG_ALPHA);
    flameModel->setParam(SPK::PARAM_RED, 0.3f, 0.6f);
    flameModel->setParam(SPK::PARAM_GREEN, 1.0f, 1.0f);
    flameModel->setParam(SPK::PARAM_BLUE, 1.0f, 1.0f);
    flameModel->setParam(SPK::PARAM_TEXTURE_INDEX, 0.0f, 4.0f);
    flameModel->setParam(SPK::PARAM_ANGLE, 0.0f, osg::PI * 0.5f, 0.0f, osg::PI * 0.5f);
    flameModel->setLifeTime(1.5f, 2.0f);
    flameModel->setShared(true);

    interpolator = flameModel->getInterpolator(SPK::PARAM_SIZE);
    interpolator->addEntry(0.0f, 0.25f);
    interpolator->addEntry(0.02f, 0.6f, 0.8f);
    interpolator->addEntry(1.0f, 1.0f, 1.4f);

    interpolator = flameModel->getInterpolator(SPK::PARAM_ALPHA);
    interpolator->addEntry(0.5f, 1.0f);
    interpolator->addEntry(1.0f, 0.0f);

    // flash model
    SPK::Model* flashModel = SPK::Model::create(
        SPK::FLAG_ALPHA | SPK::FLAG_SIZE | SPK::FLAG_ANGLE,
        SPK::FLAG_NONE,
        SPK::FLAG_ANGLE,
        SPK::FLAG_ALPHA | SPK::FLAG_SIZE);
    flashModel->setParam(SPK::PARAM_ANGLE, 0.0f, 2.0f * osg::PI);
    flashModel->setLifeTime(0.5f, 0.5f);
    flashModel->setShared(true);

    interpolator = flashModel->getInterpolator(SPK::PARAM_SIZE);
    interpolator->addEntry(0.0f, 0.25f);
    interpolator->addEntry(0.1f, 1.0f, 2.0f);

    interpolator = flashModel->getInterpolator(SPK::PARAM_ALPHA);
    interpolator->addEntry(0.0f, 1.0f);
    interpolator->addEntry(0.4f, 0.0f);

    // spark 1 model
    SPK::Model* spark1Model = SPK::Model::create(
        SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA | SPK::FLAG_SIZE,
        SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA,
        SPK::FLAG_SIZE);
    spark1Model->setParam(SPK::PARAM_RED, 0.6f);
    spark1Model->setParam(SPK::PARAM_GREEN, 1.0f);
    spark1Model->setParam(SPK::PARAM_BLUE, 1.0f);
    spark1Model->setParam(SPK::PARAM_ALPHA, 1.0f, 0.0f);
    spark1Model->setParam(SPK::PARAM_SIZE, 0.2f, 0.4f);
    spark1Model->setLifeTime(0.7f, 1.5f);
    spark1Model->setShared(true);

    // spark 2 model
    SPK::Model* spark2Model = SPK::Model::create(
        SPK::FLAG_RED | SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA,
        SPK::FLAG_GREEN | SPK::FLAG_BLUE | SPK::FLAG_ALPHA,
        SPK::FLAG_GREEN);
    spark2Model->setParam(SPK::PARAM_ALPHA, 1.0f, 0.0f);
    spark2Model->setParam(SPK::PARAM_RED, 1.0f);
    spark2Model->setParam(SPK::PARAM_GREEN, 1.0f, 1.0f, 0.3f, 1.0f);
    spark2Model->setParam(SPK::PARAM_BLUE, 0.7f, 0.3f);
    spark2Model->setLifeTime(1.0f, 3.0f);
    spark2Model->setShared(true);

    // wave model
    SPK::Model* waveModel = SPK::Model::create(
        SPK::FLAG_ALPHA | SPK::FLAG_SIZE,
        SPK::FLAG_SIZE | SPK::FLAG_ALPHA);
    waveModel->setParam(SPK::PARAM_SIZE, 0.0f, 8.0f);
    waveModel->setParam(SPK::PARAM_ALPHA, 0.2f, 0.0f);
    waveModel->setLifeTime(0.3f, 0.3f);
    waveModel->setShared(true);

    //
    // Emitters
    //
    SPK::Sphere* explosionSphere = SPK::Sphere::create(SPK::Vector3D(0.0f, 0.0f, 0.0f), 1.2f);

    // smoke emitter
    SPK::RandomEmitter* smokeEmitter = SPK::RandomEmitter::create();
    smokeEmitter->setZone(SPK::Sphere::create(SPK::Vector3D(0.0f, 0.0f, 0.0f), 1.4f), false);
    smokeEmitter->setFlow(-1);
    smokeEmitter->setTank(135);
    smokeEmitter->setForce(0.02f, 0.04f);

    // flame emitter
    SPK::NormalEmitter* flameEmitter = SPK::NormalEmitter::create();
    flameEmitter->setZone(explosionSphere);
    flameEmitter->setFlow(-1);
    flameEmitter->setTank(135);
    flameEmitter->setForce(0.06f, 0.1f);

    // flash emitter
    SPK::StaticEmitter* flashEmitter = SPK::StaticEmitter::create();
    flashEmitter->setZone(SPK::Sphere::create(SPK::Vector3D(0.0f, 0.0f, 0.0f), 0.3f));
    flashEmitter->setFlow(-1);
    flashEmitter->setTank(27);

    // spark 1 emitter
    SPK::NormalEmitter* spark1Emitter = SPK::NormalEmitter::create();
    spark1Emitter->setZone(explosionSphere);
    spark1Emitter->setFlow(-1);
    spark1Emitter->setTank(180);
    spark1Emitter->setForce(2.0f, 3.0f);

    // spark 2 emitter
    SPK::NormalEmitter* spark2Emitter = SPK::NormalEmitter::create();
    spark2Emitter->setZone(explosionSphere);
    spark2Emitter->setFlow(-1);
    spark2Emitter->setTank(3600);
    spark2Emitter->setForce(0.4f, 0.8f);

    // wave emitter
    SPK::StaticEmitter* waveEmitter = SPK::StaticEmitter::create();
    waveEmitter->setZone(SPK::Point::create());
    waveEmitter->setFlow(-1);
    waveEmitter->setTank(1);

    //
    // Groups
    //

    // smoke group
    SPK::Group* smokeGroup = SPK::Group::create(smokeModel, 135);
    smokeGroup->addEmitter(smokeEmitter);
    smokeGroup->setRenderer(smokeRenderer);
    smokeGroup->setGravity(SPK::Vector3D(0.0f, 0.0f, 0.05f));

    // flame group
    SPK::Group* flameGroup = SPK::Group::create(flameModel, 135);
    flameGroup->addEmitter(flameEmitter);
    flameGroup->setRenderer(flameRenderer);

    // flash group
    SPK::Group* flashGroup = SPK::Group::create(flashModel, 27);
    flashGroup->addEmitter(flashEmitter);
    flashGroup->setRenderer(flashRenderer);

    // spark 1 group
    SPK::Group* spark1Group = SPK::Group::create(spark1Model, 180);
    spark1Group->addEmitter(spark1Emitter);
    spark1Group->setRenderer(spark1Renderer);
    spark1Group->setGravity(SPK::Vector3D(0.0f, 0.0f, -1.5f));

    // spark 2 group
    SPK::Group* spark2Group = SPK::Group::create(spark2Model, 3600);
    spark2Group->addEmitter(spark2Emitter);
    spark2Group->setRenderer(spark2Renderer);
    spark2Group->setGravity(SPK::Vector3D(0.0f, 0.0f, -0.3f));
    spark2Group->setFriction(0.4f);

    // wave group
    SPK::Group* waveGroup = SPK::Group::create(waveModel, 1);
    waveGroup->addEmitter(waveEmitter);
    waveGroup->setRenderer(waveRenderer);

    //
    // System
    //
    SPK::System* system = SPK::System::create();
    system->addGroup(waveGroup);
    system->addGroup(smokeGroup);
    system->addGroup(flameGroup);
    system->addGroup(flashGroup);
    system->addGroup(spark1Group);
    system->addGroup(spark2Group);
    return system->getSPKID();

}
