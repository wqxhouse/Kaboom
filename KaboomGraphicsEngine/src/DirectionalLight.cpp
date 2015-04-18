#include "stdafx.h" 
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string &name)
	: Light(name)
{
	// actually this line does nothing
	// since directional light does not have a bound
	_bound.setLight(this, BOX);
	_position = osg::Vec3();
}

DirectionalLight::~DirectionalLight()
{
}

enum LightType DirectionalLight::getLightType()
{
	return DIRECTIONAL;
}

DirectionalLight *DirectionalLight::asDirectionalLight()
{
	return this;
}


void DirectionalLight::setPosition(const osg::Vec3 &pos)
{
	// no need to set position since no position
	// no need to update bounds since no bound
}