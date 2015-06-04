#pragma once
#include "ParticleEffect.h"
#include "SparkUpdatingHandler.h"
#include "Light.h"
#include "GeometryObject.h"

class TrailingEffect : public ParticleEffect
{
public:
	TrailingEffect(ParticleEffectManager *manager, SparkUpdatingHandler *handler, float r, float g, float b);
	void setTrailedObject(GeometryObject *geom);
	void setTrailedObject(Light *light);

private:
	static SPK::SPK_ID createTrailingEffect(
		const SparkDrawable::TextureIDMap& textureIDMap,
		int screenWidth, int screenHeight);

	SparkUpdatingHandler *_handler;
	GeometryObject *_trailedObject;

    float r;
    float g;
    float b;
};