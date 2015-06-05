#include "CubemapProbeManager.h"
#include <osg/Depth>
#include <util/ConfigSettings.h>
#include <osgDB/ReadFile>

CubemapProbeManager::CubemapProbeManager()
{
	_probeGroup = new osg::Group;

	osg::ref_ptr<osg::Depth> depth = new osg::Depth;
	depth->setFunction(osg::Depth::ALWAYS);
	depth->setWriteMask(false);

	ConfigSettings *config = ConfigSettings::config;
	std::string mediaPath;
	config->getValue("MediaPath", mediaPath);
	_program = new osg::Program;
	osg::Shader *vert = osgDB::readShaderFile(mediaPath + "Shaders\\defaultQuad.vert");
	osg::Shader *frag = osgDB::readShaderFile(mediaPath + "Shaders\\copyPixel.frag");
	_program->addShader(vert);
	_program->addShader(frag);
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
	_passes->setCurrentTechnique("cubemap_capture");
	for (int i = 0; i < _probes.size(); i++)
	{
		_probes[i]->enableCompute();
	}
}

void CubemapProbeManager::disableComputing()
{
	_passes->setCurrentTechnique("deferred_shading");
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
	CubemapProbe *probe = new CubemapProbe(_passes, _program);
	_probes.push_back(probe);
	_probeGroup->addChild(probe->getRoot());
	return _probes.size() - 1;
}
