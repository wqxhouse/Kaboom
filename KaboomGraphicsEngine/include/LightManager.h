#pragma once
#include <vector>
#include <unordered_map>
#include <osg/Vec3>
#include <osg/Group>
#include <osg/OcclusionQueryNode>
#include "ShadowManager.h"


namespace osgFX
{
	class EffectCompositor;
}

class LightVisualizer;
class Light;
class LightManager
{
public:
	LightManager();
	~LightManager();

	void initShadowManager(osgFX::EffectCompositor *passes, osg::Group *geomRoot);
	
	bool addDirectionalLight(const std::string &name,
		const osg::Vec3 &dirToWorld,
		const osg::Vec3 &color,
		bool castShadow,
		float intensity = 1.0f);

	bool addPointLight(const std::string &name,
		const osg::Vec3 &position,
		const osg::Vec3 &color,
		float radius, 
		bool castShadow,
		float intensity = 1.0f);

	bool setPointLightCastShadow(PointLight *pl, bool tf);

	void deleteLight(const std::string &name);
	bool renameLight(const std::string &oldName, const std::string &newName);

	bool doesNameExist(const std::string &name);

	inline const std::unordered_map<std::string, Light *> getLightMapRef() const
	{
		return _lightsMap;
	}

	inline int getNumLights()
	{
		return _numLights;
	}

	ShadowManager *getShadowManager()
	{
		return _shadowManager;
	}

	osg::ref_ptr<osg::MatrixTransform> getVisualizerRoot();
	Light *getLight(const std::string &name);
	Light *getLight(int index);

	// TODO: refactor
	DirectionalLight *getSunLight();

	bool getPointLightOcclusionResult(PointLight *pt);

	inline osg::ref_ptr<osg::Group> getPointLightOcclusionTestGroup()
	{
		return _pointLightOcclusionTestGroup;
	}

private:
	void configOcclusionSphere();
	void addPointLightToOcclusionQuery(PointLight *pt);

	std::unordered_map<std::string, Light *> _lightsMap;
	std::unordered_map<std::string, osg::ref_ptr<osg::OcclusionQueryNode> > _ocQueryMap;
	std::vector<Light *> _lights;
	int _numLights;

	LightVisualizer *_visualizer;
	ShadowManager *_shadowManager;

	DirectionalLight *_sunLight;

	osg::ref_ptr<osg::Group> _pointLightOcclusionTestGroup;

	static osg::ref_ptr<osg::Node> _occlusionSphere;
};

