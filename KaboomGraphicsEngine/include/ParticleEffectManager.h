#pragma once
#include <unordered_map>
#include "ParticleTexture.h"
#include "ParticleEffect.h"
#include "SparkUpdatingHandler.h"

class ParticleEffectManager
{
public:
	enum BuiltInParticleEffect
	{
		EXPLOSION,
        EXPLOSION2,
		TRAILING,
	};

	ParticleEffectManager();
	~ParticleEffectManager();

	// void addParticle(const std::string &name);

	inline osg::ref_ptr<osg::Group> getRoot()
	{
		return _particleGroup;
	}

	// TODO: consider make it accessible only to ParticleEffect class
	ParticleTexture *getOrCreateParticleTexture(const std::string &path);
	ParticleEffect *getParticleEffect(enum BuiltInParticleEffect effect);

	osg::ref_ptr<SparkUpdatingHandler> getParticleUpdateHandler();

	void createBuiltInParticleEffects();

private:
    void createBuiltInExplosionEffect();
    void createBuiltInExplosionEffect2();
	void createBuiltInTrailingEffect();

	std::unordered_map<std::string, ParticleTexture *> _particleTextures;
	std::unordered_map<std::string, ParticleEffect *> _particleEffects;
	osg::ref_ptr<osg::Group> _particleGroup;
	osg::ref_ptr<SparkUpdatingHandler> _particleUpdateHandler;

    const std::string _builtInExplsionEffectStr;
    const std::string _builtInExplsionEffect2Str;
	const std::string _builtInTrailingEffectStr;
};