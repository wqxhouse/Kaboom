#include "stdafx.h" 

#include "LightManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "LightVisualizer.h"

LightManager::LightManager()
	: _shadowManager(NULL), _sunLight(NULL)
{
	_visualizer = new LightVisualizer();
}

LightManager::~LightManager()
{
	for (std::unordered_map<std::string, Light *>::iterator it = _lightsMap.begin(); 
		it != _lightsMap.end(); ++it)
	{
		delete it->second;
	}
	_lightsMap.clear();

	delete _visualizer;

	if (_shadowManager != NULL)
	{
		delete _shadowManager;
	}
}

void LightManager::initShadowManager(osgFX::EffectCompositor *passes, osg::Group *geomRoot)
{
	_shadowManager = new ShadowManager(passes, geomRoot);
}

bool LightManager::addDirectionalLight(const std::string &name,
	const osg::Vec3 &dirToWorld,
	const osg::Vec3 &color,
	bool castShadow,
	float intensity)
{
	// Handle duplicated (name) geoms
	if (doesNameExist(name)) {
		std::cout << "Name already exists: " << name << std::endl;
		return false;
	}

	DirectionalLight *dirLight = new DirectionalLight(name);
	dirLight->setColor(color);
	dirLight->setLightToWorldDirection(dirToWorld);
	dirLight->setCastShadow(castShadow);
	dirLight->setIntensity(intensity);

	_lightsMap.insert(std::make_pair(name, dirLight));
	_lights.push_back(dirLight);

	if (castShadow && _shadowManager != NULL)
	{
		_shadowManager->addDirectionalLight(dirLight);
	}

	// add the first directional light to be sun light
	// TODO : later support specifying which dirlight is sunlight
	// or have only one directional light in the manager and make it
	// the sun light. (though currently the shader can only have 1 
	// directional light.
	if (_sunLight == NULL)
	{
		_sunLight = dirLight;
	}

	++_numLights;

	return true;
}

bool LightManager::addPointLight(const std::string &name,
	const osg::Vec3 &position,
	const osg::Vec3 &color,
	float radius, 
	bool castShadow,
	float intensity)
{
	// Handle duplicated (name) geoms
	if (doesNameExist(name)) {
		std::cout << "Name already exists: " << name << std::endl;
		return false;
	}

	PointLight *pointLight = new PointLight(name);
	pointLight->setPosition(position);
	pointLight->setColor(color);
	pointLight->setRadius(radius);
	pointLight->setCastShadow(castShadow);
	pointLight->setIntensity(intensity);

	_visualizer->addLight(pointLight);

	_lightsMap.insert(std::make_pair(name, pointLight));
	_lights.push_back(pointLight);

	if (castShadow && _shadowManager != NULL)
	{
		_shadowManager->addPointLight(pointLight);
	}

	++_numLights;

	return true;
}

void LightManager::deleteLight(const std::string &name)
{
	Light *light = _lightsMap[name];

	_lightsMap.erase(name);
	_lights.erase(std::remove(_lights.begin(), _lights.end(), light), _lights.end());

	_visualizer->removeLight(light);

	PointLight *pl = NULL;
	if (light->getCastShadow() && ((pl = light->asPointLight()) != NULL))
	{
		_shadowManager->removePointLight(pl);
	}

	delete light;

	--_numLights;
}

DirectionalLight *LightManager::getSunLight()
{
	return _sunLight;
}

bool LightManager::renameLight(const std::string &oldName, const std::string &newName)
{	
	// Handle duplicated (name) geoms
	if (doesNameExist(newName)) {
		std::cout << "Name already exists: " << newName << std::endl;
		return false;
	}

	Light *light = _lightsMap[oldName];
	_lightsMap.erase(oldName);

	light->setName(newName);
	_lightsMap.insert(std::make_pair(newName, light));
	return true;
}

bool LightManager::doesNameExist(const std::string &name)
{
	std::unordered_map<std::string, Light *>::const_iterator got =
		_lightsMap.find(name);

	// If the name already exists
	if (got != _lightsMap.end()) {
		return true;
	}
	return false;
}

osg::ref_ptr<osg::MatrixTransform> LightManager::getVisualizerRoot()
{
	return _visualizer->getRoot();
}

Light *LightManager::getLight(const std::string &name)
{
	std::unordered_map<std::string, Light *>::iterator it = _lightsMap.find(name);
	if (it != _lightsMap.end())
	{
		return it->second;
	}
	else
	{
		// output not
		return NULL;
	}
}

Light *LightManager::getLight(int index)
{
	if (index >= _numLights || index < 0)
	{
		// TODO: display on console the error : out of bounds
		return NULL;
	}
	return _lights[index];
}