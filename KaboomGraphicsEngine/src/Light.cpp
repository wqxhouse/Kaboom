#include "stdafx.h" 

#include "Light.h"

Light::Light(const std::string &name)
	: _name(name), _intensity(1.0)
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
