#pragma once
#include "ParticleEffect.h"

class ParticleEffectManager;
class ExplosionEffect : public ParticleEffect
{
public:
	ExplosionEffect(ParticleEffectManager *manager);

private:
	static SPK::SPK_ID createExplosionEffect(
		const SparkDrawable::TextureIDMap& textureIDMap, 
		int screenWidth, int screenHeight);
};
