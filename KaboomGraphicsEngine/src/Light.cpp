#include "stdafx.h" 

#include "Light.h"

Light::Light(const std::string &name)
	: _name(name)
{
}

Light::~Light()
{
}

enum LightType Light::getLightType()
{
	return BASE;
}

DirectionalLight *Light::asDirectionalLight()
{
	return NULL;
}

PointLight *Light::asPointLight()
{
	return NULL;
}
