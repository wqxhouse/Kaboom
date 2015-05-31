#include "stdafx.h" 
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string &name)
	: Light(name)
{
	// actually this line does nothing
	// since directional light does not have a bound
	_bound.setLight(this, BOX);
	_position = osg::Vec3();

	initShadowMapInfo();
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


void DirectionalLight::setShadowAtlasPos(int split, const osg::Vec2i &pos)
{
	_shadowMapInfo[split]._atlasPos = pos;
	_shadowMapInfo[split]._hasAtlasPos = true;
}

osg::Vec2i DirectionalLight::getShadowAtlasPos(int split)
{
	return _shadowMapInfo[split]._atlasPos;
}

bool DirectionalLight::hasShadowMapAtlasPos(int split)
{
	return _shadowMapInfo[split]._hasAtlasPos;
}

void DirectionalLight::setShadowMapIndex(int split, int index)
{
	_shadowMapInfo[split]._shadowMapIndex = index;
}

int DirectionalLight::getShadowMapIndex(int split)
{
	return _shadowMapInfo[split]._shadowMapIndex;
}

void DirectionalLight::initShadowMapInfo()
{
	// TODO: change back to pssm if have time
	//_shadowMapRes = 1024;
	//_shadowNumSplits = 3; // default

	_shadowMapRes = 2048;
	_shadowNumSplits = 1;

	_cascadeSplitDists.resize(_shadowNumSplits);
	_shadowMapInfo.resize(_shadowNumSplits);
	_shadowFarPlane = 300; // default
	for (int i = 0; i < _shadowNumSplits; i++)
	{
		_shadowMapInfo[i]._atlasPos = osg::Vec2i(-1, -1);
		_shadowMapInfo[i]._hasAtlasPos = false;
		_shadowMapInfo[i]._shadowMapIndex = -1;
	}
}