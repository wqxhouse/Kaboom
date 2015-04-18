#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const std::string &name);
	virtual ~PointLight();

	virtual LightType getLightType();

	inline float getRadius()
	{
		return _radius;
	}

	void setRadius(float radius);

	virtual PointLight *asPointLight();

	// override setPosition for updating bbox
	virtual void setPosition(const osg::Vec3 &pos);

private:
	float _radius;
};

