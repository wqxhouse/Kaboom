#include "stdafx.h" 
#include "PointLight.h"

PointLight::PointLight(const std::string &name)
	: Light(name)
{
	_bound.setLight(this, SPHERE);
	_shadowMapInfo.resize(6); // 6 faces
	_shadowMapRes = 512;
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


void PointLight::setShadowAtlasPos(int face, const osg::Vec2 &pos)
{
	_shadowMapInfo[face].atlasPos = pos;
}

osg::Vec2 PointLight::getShadowAtlasPos(int face)
{
	return _shadowMapInfo[face].atlasPos;
}


void PointLight::setShadowMapIndex(int face, int index)
{
	_shadowMapInfo[face]._shadowMapIndex = index;
}

int PointLight::getShadowMapIndex(int face)
{
	return _shadowMapInfo[face]._shadowMapIndex;
}