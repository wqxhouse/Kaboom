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

	osg::ref_ptr<osg::Group> _probeGroup;
};