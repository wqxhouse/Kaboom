#include "stdafx.h" 

#include "LightManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"

LightManager::LightManager()
{
}


LightManager::~LightManager()
{
	for (std::unordered_map<std::string, Light *>::iterator it = _lightsMap.begin(); 
		it != _lightsMap.end(); ++it)
	{
		delete it->second;
	}
	_lightsMap.clear();
}

void LightManager::addDirectionalLight(const std::string &name,
	const osg::Vec3 &dirToWorld,
	const osg::Vec3 &color,
	bool castShadow)
{
	DirectionalLight *dirLight = new DirectionalLight(name);
	dirLight->setColor(color);
	dirLight->setLightToWorldDirection(dirToWorld);
	dirLight->setCastShadow(castShadow);

	_lightsMap.insert(std::make_pair(name, dirLight));
	_lights.push_back(dirLight);
	++_numLights;
}

void LightManager::addPointLight(const std::string &name,
	const osg::Vec3 &position,
	const osg::Vec3 &color,
	float radius, 
	bool castShadow)
{
	PointLight *pointLight = new PointLight(name);
	pointLight->setPosition(position);
	pointLight->setColor(color);
	pointLight->setRadius(radius);
	pointLight->setCastShadow(castShadow);

	_lightsMap.insert(std::make_pair(name, pointLight));
	_lights.push_back(pointLight);
	++_numLights;
}

void LightManager::deleteLight(const std::string &name)
{
	Light *light = _lightsMap[name];

	_lightsMap.erase(name);
	_lights.erase(std::remove(_lights.begin(), _lights.end(), light), _lights.end());

	delete light;

	--_numLights;
}

void LightManager::renameLight(const std::string &oldName, const std::string &newName)
{
	Light *light = _lightsMap[oldName];
	_lightsMap.erase(oldName);

	light->setName(newName);
	_lightsMap.insert(std::make_pair(newName, light));
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