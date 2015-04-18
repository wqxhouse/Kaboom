#pragma once
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/Polytope>

class Light;
enum LightBoundType
{
	BOX, 
	SPHERE,
};

class LightBound
{
public:
	LightBound();
	~LightBound();

	bool intersectBound(const osg::BoundingBox &bbox) const;
	bool intersectBound(const osg::BoundingSphere &bsphere) const;

	// cannot use const since API does not have a const version of contains
	bool intersectBound(const osg::Polytope &polyTope) const;
	void setLight(Light *light, LightBoundType type);

private:
	osg::BoundingBox _bbox;
	osg::BoundingSphere _bsphere;

	Light *_light;
	enum LightBoundType _boundType;
};
