#pragma once
#include <vector>
#include <osg/BoundingSphere>
#include <osg/TextureCubeMap>
#include "EffectCompositor.h"

class CubemapProbe
{
public:
	CubemapProbe(osgFX::EffectCompositor *passes);
	
private:
	void setupProbeCamera();
	void setupCubeMap();
	void setupPasses();
	void disablePasses();
	void updateProbeCamera();

	void enableCompute();
	void disableCompute();

	osg::Matrix calcViewMatrix(int axis, const osg::Vec3 &eyePos);

	void setPosition(const osg::Vec3 &position);
	void setRadius(float radius);

	float _radius;
	osg::Vec3 _position;
	osg::BoundingSphere _bsphere;

	std::vector<osg::ref_ptr<osg::Camera> > _cameraList;
	osgFX::EffectCompositor *_passes;
	osg::ref_ptr<osg::TextureCubeMap> _sampleCube;

	bool _isInit;
};

