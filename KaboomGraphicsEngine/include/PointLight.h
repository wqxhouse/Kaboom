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
	void setShadowMapRes(int res)
	{
		_shadowMapRes = res;
	}

	int getShadowMapRes()
	{
		return _shadowMapRes;
	}

	void setShadowAtlasPos(int face, const osg::Vec2i &pos);
	osg::Vec2i getShadowAtlasPos(int face);

	void setShadowMapIndex(int face, int index);
	int getShadowMapIndex(int face);

	bool hasShadowMapAtlasPos(int face);

	virtual PointLight *asPointLight();

	// override setPosition for updating bbox
	virtual void setPosition(const osg::Vec3 &pos);
	void resetShadowMapProperities();

	void pickRandomRotationAxis();

	inline osg::Vec3 getRotationAxis()
	{
		return _rotatingAxis;
	}

private:
	void initShadowMapInfo();

	float _radius;
	int _shadowMapRes;
	osg::Vec3 _rotatingAxis;

	// indexed by cube faces
	std::vector<ShadowMapInfo> _shadowMapInfo; 
};

