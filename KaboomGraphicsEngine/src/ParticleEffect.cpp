#include "ParticleEffect.h"

#include <osg/Geode>
#include "ParticleEffectManager.h"

ParticleEffect::ParticleEffect(ParticleEffectManager *manager)
	: _manager(manager)
{
	_particleRoot = new osg::Geode;
	_particleRoot->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	_particleRoot->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	_particleDrawable = new SparkDrawable;
	_particleRoot->addDrawable(_particleDrawable);
}

void ParticleEffect::run(const osg::Vec3& p /* = osg::Vec3() */, const osg::Quat& r /* = osg::Quat() */)
{
	_particleDrawable->addParticleSystem(p, r);
}

int ParticleEffect::getTextureIdFromTextureIDMap(
	const SparkDrawable::TextureIDMap& textureIDMap, 
	const std::string &name)
{
    GLuint var = 0; 
	auto itr = textureIDMap.find(name);
	if (itr != textureIDMap.end())
	{
		var = itr->second;
		return var;
	}
	else
	{
		OSG_WARN << "ParticleEffect:: texture named " << name << " not found in textureIDMap" << std::endl;
		return -1000;
	}
}