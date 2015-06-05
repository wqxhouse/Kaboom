#include "CubemapProbeManager.h"

CubemapProbeManager::~CubemapProbeManager()
{
	for (int i = 0; i < _probes.size(); i++)
	{
		delete _probes[i];
	}
}

void CubemapProbeManager::init(osgFX::EffectCompositor *passes)
{
	_passes = passes;
}

int CubemapProbeManager::addProbe(const osg::Vec3 &pos, float radius)
{
	CubemapProbe *probe = new CubemapProbe(_passes);
	_probes.push_back(probe);
	return _probes.size() - 1;
}