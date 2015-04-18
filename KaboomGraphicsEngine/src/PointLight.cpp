#include "stdafx.h" 
#include "PointLight.h"

PointLight::PointLight(const std::string &name)
	: Light(name)
{
	_bound.setLight(this, SPHERE);
}

PointLight::~PointLight()
{
}

enum LightType PointLight::getLightType()
{
	return POINT;
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