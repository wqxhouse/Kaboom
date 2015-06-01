#include "stdafx.h" 
#include "PointLight.h"
#include "Core.h"

PointLight::PointLight(const std::string &name)
	: Light(name)
{
	_bound.setLight(this, SPHERE);
	_shadowMapInfo.resize(6); // 6 faces
	_shadowMapRes = 512;

	initShadowMapInfo();
}

PointLight::~PointLight()
{
}

enum LightType PointLight::getLightType()
{
	return POINTLIGHT;
}

PointLight *PointLight::asPointLight()
{
	return this;
}

void PointLight::setPosition(const osg::Vec3 &pos)
{
	_position = pos;
	
	// update bound
	_bound.setLight(this, SPHERE);
}

void PointLight::setRadius(float radius)
{
	if (_radius < 0)
	{
		_radius = 0;

		return;
	}
	_radius = radius;

	// update bound
	_bound.setLight(this, SPHERE);
}


void PointLight::setShadowAtlasPos(int face, const osg::Vec2i &pos)
{
	_shadowMapInfo[face]._atlasPos = pos;
	_shadowMapInfo[face]._hasAtlasPos = true;
}

osg::Vec2i PointLight::getShadowAtlasPos(int face)
{
	return _shadowMapInfo[face]._atlasPos;
}

bool PointLight::hasShadowMapAtlasPos(int face)
{
	return _shadowMapInfo[face]._hasAtlasPos;
}

void PointLight::setShadowMapIndex(int face, int index)
{
	_shadowMapInfo[face]._shadowMapIndex = index;
}

int PointLight::getShadowMapIndex(int face)
{
	return _shadowMapInfo[face]._shadowMapIndex;
}

void PointLight::initShadowMapInfo()
{
	for (int i = 0; i < 6; i++)
	{
		_shadowMapInfo[i]._atlasPos = osg::Vec2i(-1, -1);
		_shadowMapInfo[i]._hasAtlasPos = false;
		_shadowMapInfo[i]._shadowMapIndex = -1;
	}
}
