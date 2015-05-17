#pragma once
#include "ParticleEffect.h"

class ParticleEffectManager;
class ExplosionEffect : public ParticleEffect
{
public:
	ExplosionEffect(ParticleEffectManager *manager);
	// void setExplosionRadius(float radius);
	// void run(const osg::Vec3& p = osg::Vec3(), const osg::Quat& r = osg::Quat());

private:
	static SPK::SPK_ID createExplosionEffect(
		const SparkDrawable::TextureIDMap& textureIDMap, 
		int screenWidth, int screenHeight);
};
