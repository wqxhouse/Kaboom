#include "CubemapProbeManager.h"

CubemapProbeManager::CubemapProbeManager()
{
	_probeGroup = new osg::Group;
}

CubemapProbeManager::~CubemapProbeManager()
{
	for (int i = 0; i < _probes.size(); i++)
	{
		delete _probes[i];
	}
}


void CubemapProbeManager::enableComputing()
{
	for (int i = 0; i < _probes.size(); i++)
	{
		_probes[i]->enableCompute();
	}
}

void CubemapProbeManager::disableComputing()
{
	for (int i = 0; i < _probes.size(); i++)
	{
		_probes[i]->disableCompute();
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
	_probeGroup->addChild(probe->getRoot());
	return _probes.size() - 1;
}
