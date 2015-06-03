#pragma once
#include "ParticleEffect.h"

class ParticleEffectManager;
class ExplosionEffect2 : public ParticleEffect
{
public:
	ExplosionEffect2(ParticleEffectManager *manager);
	// void setExplosionRadius(float radius);
	// void run(const osg::Vec3& p = osg::Vec3(), const osg::Quat& r = osg::Quat());

private:
	static SPK::SPK_ID createExplosionEffect2(
		const SparkDrawable::TextureIDMap& textureIDMap, 
		int screenWidth, int screenHeight);
};
