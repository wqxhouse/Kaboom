#include "stdafx.h" 

#include "LightBound.h"
#include "PointLight.h"

LightBound::LightBound()
{
}

LightBound::~LightBound()
{
}

void LightBound::setLight(Light *light, enum LightBoundType boundType)
{
	_light = light;
	_boundType = boundType;

	if (_light->getLightType() == POINT)
	{
		PointLight *pl = light->asPointLight();
		if (_boundType == BOX)
		{
			float radius = pl->getRadius();
			const osg::Vec3 &pos = pl->getPosition();
			osg::Vec3 min = osg::Vec3(pos.x() - radius, pos.y() - radius, pos.z() - radius);
			osg::Vec3 max = osg::Vec3(pos.x() + radius, pos.y() + radius, pos.z() + radius);
			_bbox.set(min, max);
		}
		else if (_boundType == SPHERE)
		{
			_bsphere.set(light->getPosition(), pl->getRadius());
		}
		else
		{
			// TODO: to be implemented
		}
	}
	else
	{
		// TODO: to be implemented
	}
}

// cannot use const since API does not have a const version of contains
bool LightBound::intersectBound(const osg::Polytope &polyTope) const
{
	osg::Polytope cloned = polyTope;
	if (_light->getLightType() == POINT)
	{
		bool result;
		if (_boundType == SPHERE)
		{
			result = cloned.contains(_bsphere);
		}
		else if (_boundType == BOX)
		{
			result = cloned.contains(_bbox);
		}
		return result;
	}
	else if (_light->getLightType() == DIRECTIONAL)
	{
		// directional light always pass tests
		return true;
	}
	else 
	{
		// TODO : to be implemented
		return false;
	}
}

bool LightBound::intersectBound(const osg::BoundingSphere &bsphere) const
{
	// TODO: implement
	return false;
}

bool LightBound::intersectBound(const osg::BoundingBox &bbox) const
{
	// TODO: implement
	return false;
}