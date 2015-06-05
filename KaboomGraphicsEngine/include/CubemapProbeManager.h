#pragma once
#include <vector>
#include "EffectCompositor.h"
#include "CubemapProbe.h"

class CubemapProbeManager
{
public:
	CubemapProbeManager();
	~CubemapProbeManager();

	inline osg::ref_ptr<osg::Group> getRoot()
	{
		return _probeGroup;
	}

	void enableComputing();
	void disableComputing();

	int addProbe(const osg::Vec3 &pos, float radius);
	void init(osgFX::EffectCompositor *passes);

private:
	std::vector<CubemapProbe *> _probes;
	osgFX::EffectCompositor *_passes;

	osg::observer_ptr<osg::Texture> _shadingBuffer;

	osg::ref_ptr<osg::Group> _probeGroup;
	osg::ref_ptr<osg::Program> _program;
};