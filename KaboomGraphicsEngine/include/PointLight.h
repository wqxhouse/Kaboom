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

	void setShadowAtlasPos(int face, const osg::Vec2 &pos);
	osg::Vec2 getShadowAtlasPos(int face);

	void setShadowMapIndex(int face, int index);
	int getShadowMapIndex(int face);

	virtual PointLight *asPointLight();

	// override setPosition for updating bbox
	virtual void setPosition(const osg::Vec3 &pos);

private:


	float _radius;
	int _shadowMapRes;

	// indexed by cube faces
	std::vector<ShadowMapInfo> _shadowMapInfo; 
};

