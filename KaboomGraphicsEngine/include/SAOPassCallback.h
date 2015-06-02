#pragma once
#include <osg/StateSet>

// TODO: separate callback from manager
class SAOPassCallback : public osg::StateSet::Callback
{
public:
	SAOPassCallback(osg::Camera *saoPassCam);
	virtual void operator()(osg::StateSet*, osg::NodeVisitor*);

	inline void setIntensity(float intensity)
	{
		_intensity = intensity;
		_intensityDivRPow6 = _intensity / pow(_radius, 6.0f);
	}

	inline float getIntensity()
	{
		return _intensity;
	}

	inline void setRadius(float radius)
	{
		_radius = radius;
		_intensityDivRPow6 = _intensity / pow(_radius, 6.0f);
	}

	inline float getRadius()
	{
		return _radius;
	}

private:
	float calcProjScale(float fovy, float viewportHeight);
	float _intensity;
	float _radius;
	float _intensityDivRPow6;

	osg::observer_ptr<osg::Camera> _saoPassCam;
};