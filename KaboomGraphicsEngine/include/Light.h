#pragma once

#include <osg/Vec3>
#include "LightBound.h"

enum LightType
{
	BASE,
	DIRECTIONAL,
	POINTLIGHT,
	SPOT,
	AREA,
};

class DirectionalLight;
class PointLight;
class Light 
{
public:
	Light(const std::string &name);
	virtual ~Light();

	virtual enum LightType getLightType();

	// need to be overridden for setting bounds
	virtual void setPosition(const osg::Vec3 &pos) = 0;

	inline const osg::Vec3 &getPosition()
	{
		_needUpdate = true;
		return _position;
	}

	inline void setColor(const osg::Vec3 &color)
	{
		_color = color;
	}

	inline const osg::Vec3 &getColor()
	{
		return _color;
	}

	inline bool getCastShadow()
	{
		return _castShadow;
	}

	inline const LightBound &getLightBound()
	{
		return _bound;
	}

	inline std::string getName()
	{
		return _name;
	}

	inline void setName(std::string name)
	{
		_name = name;
	}

	inline float getIntensity()
	{
		return _intensity;
	}

	inline void setIntensity(float intensity)
	{
		_intensity = intensity;
	}

	inline void setNeedUpdate(bool tf)
	{
		_needUpdate = tf;
	}

	inline bool getNeedUpdate()
	{
		return _needUpdate;
	}

	virtual DirectionalLight *asDirectionalLight();
	virtual PointLight *asPointLight();

	struct ShadowMapInfo
	{
		int _shadowMapIndex;
		osg::Vec2i _atlasPos;
		bool _hasAtlasPos;
	};

	// let LightManager manage castShadow
	// bool setCastShadow(bool tf);

protected:
	friend class LightManager;

	std::string _name;
	LightBound _bound;

	osg::Vec3 _position;
	osg::Vec3 _color;

	bool _needUpdate;
	bool _castShadow;

	float _intensity;
};

