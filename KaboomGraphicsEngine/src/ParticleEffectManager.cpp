#include "ParticleEffectManager.h"
#include <osgDB/ReadFile>

#include "ExplosionEffect.h"
#include "ExplosionEffect2.h"
#include "TrailingEffect.h"

ParticleEffectManager::ParticleEffectManager()
	: _builtInExplsionEffectStr("__explosionEffect"),
      _builtInExplsionEffect2Str("__explosionEffect2"),
	  _builtInTrailingEffectStr("__trailingEffect")
{
	// TODO: make sure depth write is disabled 
	// Since, currently depth write is not managed by osg states
	_particleGroup = new osg::Group;
	_particleGroup->setNodeMask(0x10);

	_particleUpdateHandler = new SparkUpdatingHandler;
	createBuiltInParticleEffects();
}

ParticleEffectManager::~ParticleEffectManager()
{
	for (auto it : _particleTextures)
	{
		delete it.second;
	}

	_particleTextures.clear();

	for (auto it : _particleEffects)
	{
		delete it.second;
	}

	_particleEffects.clear();

	// TODO: clear graphics memory for textures
}

void ParticleEffectManager::createBuiltInParticleEffects()
{
    createBuiltInExplosionEffect();
    createBuiltInExplosionEffect2();
}

void ParticleEffectManager::createBuiltInExplosionEffect()
{
	ExplosionEffect *explosion = new ExplosionEffect(this);
	_particleEffects.insert(std::make_pair(_builtInExplsionEffectStr, explosion));
	_particleGroup->addChild(explosion->getRoot());
	// TODO: refactor
	_particleUpdateHandler->addSpark(static_cast<SparkDrawable *>(explosion->getRoot()->getDrawable(0)));
}

void ParticleEffectManager::createBuiltInExplosionEffect2()
{
	ExplosionEffect2 *explosion = new ExplosionEffect2(this);
	_particleEffects.insert(std::make_pair(_builtInExplsionEffect2Str, explosion));
	_particleGroup->addChild(explosion->getRoot());
	// TODO: refactor
	_particleUpdateHandler->addSpark(static_cast<SparkDrawable *>(explosion->getRoot()->getDrawable(0)));
}

ParticleEffect *ParticleEffectManager::getParticleEffect(enum BuiltInParticleEffect effect)
{
	TrailingEffect *trailingEffect;

	switch (effect)
    {
    case EXPLOSION:
        return _particleEffects[_builtInExplsionEffectStr];
        break;
    case EXPLOSION2:
        return _particleEffects[_builtInExplsionEffect2Str];
        break;
    case TRAILING_BLUE:
        trailingEffect = new TrailingEffect(this, _particleUpdateHandler, 0.0941f, 0.9216f, 1.0f);
        _particleGroup->addChild(trailingEffect->getRoot());
        _particleUpdateHandler->addSpark(static_cast<SparkDrawable *>(trailingEffect->getRoot()->getDrawable(0)));
        return trailingEffect;
        break;
    case TRAILING_GREEN:
        trailingEffect = new TrailingEffect(this, _particleUpdateHandler, 0.1765f, 1.0f, 0.0941f);
        _particleGroup->addChild(trailingEffect->getRoot());
        _particleUpdateHandler->addSpark(static_cast<SparkDrawable *>(trailingEffect->getRoot()->getDrawable(0)));
        return trailingEffect;
        break;
    case TRAILING_PURPLE:
        trailingEffect = new TrailingEffect(this, _particleUpdateHandler, 0.8510f, 0.0941f, 1.0f);
        _particleGroup->addChild(trailingEffect->getRoot());
        _particleUpdateHandler->addSpark(static_cast<SparkDrawable *>(trailingEffect->getRoot()->getDrawable(0)));
        return trailingEffect;
        break;
    case TRAILING_RED:
        trailingEffect = new TrailingEffect(this, _particleUpdateHandler, 1.0f, 0.2392f, 0.0941f);
        _particleGroup->addChild(trailingEffect->getRoot());
        _particleUpdateHandler->addSpark(static_cast<SparkDrawable *>(trailingEffect->getRoot()->getDrawable(0)));
        return trailingEffect;
        break;
	default:
		OSG_WARN << "ParticleEffectManager:: invalid enum" << std::endl;
		return NULL;
	}
}

osg::ref_ptr<SparkUpdatingHandler> ParticleEffectManager::getParticleUpdateHandler()
{
	return _particleUpdateHandler;
}

ParticleTexture *ParticleEffectManager::getOrCreateParticleTexture(const std::string &path)
{
	auto it = _particleTextures.find(path);
	if (it == _particleTextures.end())
	{
		osg::Image *img = osgDB::readImageFile(path);
		if (img == NULL)
		{
			OSG_WARN << "Failed to load texture at " << path << std::endl;
		}
		ParticleTexture *tex = new ParticleTexture(img);
		_particleTextures.insert(std::make_pair(path, tex));
		return tex;
	}
	else
	{
		return it->second;
	}
}