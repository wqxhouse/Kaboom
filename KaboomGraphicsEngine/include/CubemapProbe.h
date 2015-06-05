#pragma once
#include <vector>
#include <osg/BoundingSphere>
#include <osg/TextureCubeMap>
#include "EffectCompositor.h"

class CubemapProbe
{
public:
	CubemapProbe(osgFX::EffectCompositor *passes);

	inline osg::ref_ptr<osg::Group> getRoot()
	{
		return _camGroup;
	}

	void enableCompute();
	void disableCompute();
	
private:
	void setupProbeCamera();
	void setupCubeMap();
	void setupPasses();
	void disablePasses();
	void updateProbeCamera();

	osg::Matrix calcViewMatrix(int axis, const osg::Vec3 &eyePos);

	void setPosition(const osg::Vec3 &position);
	void setRadius(float radius);

	float _radius;
	osg::Vec3 _position;
	osg::BoundingSphere _bsphere;

	std::vector<osg::ref_ptr<osg::Camera> > _cameraList;
	osgFX::EffectCompositor *_passes;
	osg::ref_ptr<osg::TextureCubeMap> _sampleCube;
	osg::ref_ptr<osg::Group> _camGroup;

	bool _isInit;
};

