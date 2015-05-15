#pragma once
#include "SparkDrawable.h"

class ParticleEffectManager;
class ParticleEffect
{
public:
	ParticleEffect(ParticleEffectManager *manager);
	~ParticleEffect() {};

	inline std::string getName()
	{
		return _name;
	}

	inline osg::ref_ptr<osg::Geode> getRoot()
	{
		return _particleRoot;
	}

	void run(const osg::Vec3& p = osg::Vec3(), const osg::Quat& r = osg::Quat());

protected:
	// TODO: refactor. 
	static int getTextureIdFromTextureIDMap(
		const SparkDrawable::TextureIDMap& textureIDMap, 
		const std::string &name);

	std::string _name;
	osg::ref_ptr<osg::Geode> _particleRoot;
	osg::ref_ptr<SparkDrawable> _particleDrawable;

	ParticleEffectManager *_manager;
};