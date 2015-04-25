#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const std::string &name);
	virtual ~DirectionalLight();

	virtual enum LightType getLightType();
	
	inline void setLightToWorldDirection(const osg::Vec3 &lightToWorldDir)
	{
		_lightToWorldDirection = lightToWorldDir;
		_lightToWorldDirection.normalize();
	}

	inline const osg::Vec3 &getLightToWorldDirection()
	{
		return _lightToWorldDirection;
	}
	
	virtual DirectionalLight *asDirectionalLight();
	virtual void setPosition(const osg::Vec3 &pos);

private:
	osg::Vec3 _lightToWorldDirection;
};

