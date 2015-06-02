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

	// TODO: consider refactor this with pointlight to the base classes since
	// their signatures are actually the same
	void setShadowAtlasPos(int split, const osg::Vec2i &pos);
	osg::Vec2i getShadowAtlasPos(int split);

	void setShadowMapIndex(int split, int index);
	int getShadowMapIndexForSplit(int split);

	bool hasShadowMapAtlasPos(int split);

	inline float getShadowFarPlane()
	{
		return _shadowFarPlane;
	}

	inline int getShadowMapRes()
	{
		return _shadowMapRes;
	}

	inline void setShadowMapRes(int resolution)
	{
		_shadowMapRes = resolution;
	}

	inline int getNumSplits()
	{
		return _shadowNumSplits;
	}
	// bool setNumSplits(int splitNum);

	inline float getSlopeScaledBias()
	{
		return _slopeScaledBias;
	}

	inline float getNormalScaledBias()
	{
		return _normalScaledBias;
	}

	inline float getBaseBias()
	{
		return _baseBias;
	}

	inline void setSlopeScaledBias(float bias)
	{
		_slopeScaledBias = bias;
	}

	inline void setNormalScaledBias(float bias)
	{
		_normalScaledBias = bias;
	}

	inline void setBaseBias(float bias)
	{
		_baseBias = bias;
	}

	inline void setCascadeSplitDist(int split, float dist)
	{
		if (split >= _shadowNumSplits)
		{
			OSG_WARN << "Split out of bounds" << std::endl;
		}

		_cascadeSplitDists[split] = dist;
	}

	inline float getCascadeSplitDist(int split)
	{
		if (split >= _shadowNumSplits)
		{
			OSG_WARN << "Split out of bounds" << std::endl;
		}

		return _cascadeSplitDists[split];
	}

private:
	osg::Vec3 _lightToWorldDirection;

private:
	void initShadowMapInfo();

	// TODO: consider support different res splits 
	int _shadowMapRes;
	int _shadowNumSplits;
	int _shadowFarPlane;

	float _slopeScaledBias;
	float _normalScaledBias;
	float _baseBias;

	std::vector<float> _cascadeSplitDists;

	// indexed by cube faces
	std::vector<ShadowMapInfo> _shadowMapInfo;
};

