#include "stdafx.h" 

#include <osg/Depth>
#include <util/ConfigSettings.h>
#include "LightManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "LightVisualizer.h"

LightManager::LightManager()
	: _shadowManager(NULL), _sunLight(NULL)
{
	_visualizer = new LightVisualizer();
	_pointLightOcclusionTestGroup = new osg::Group;
	_pointLightOcclusionTestGroup->setName("pointLightOcclusionTestGroup");
	_pointLightOcclusionTestGroup->setNodeMask(0x20);
	osg::Depth *depth = new osg::Depth;
	depth->setWriteMask(false);
	// depth->setFunction(osg::Depth::ALWAYS);
	_pointLightOcclusionTestGroup->getOrCreateStateSet()->setAttribute(depth);
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

void LightManager::configOcclusionSphere()
{
	if (_occlusionSphere == NULL)
	{
		ConfigSettings *config = ConfigSettings::config;
		std::string mediaPath;
		config->getValue("MediaPath", mediaPath);
		_occlusionSphere = osgDB::readNodeFile(mediaPath + "DefaultAssets\\GeometryObject\\occlusionSphere.fbx");
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
	// dirLight->setCastShadow(castShadow);

	bool res = true;
	if (castShadow && _shadowManager != NULL)
	{
		res = _shadowManager->addDirectionalLight(dirLight);
	}

	if (res)
	{
		dirLight->_castShadow = castShadow;
	}
	else
	{
		dirLight->_castShadow = false;
	}

	dirLight->setIntensity(intensity);

	_lightsMap.insert(std::make_pair(name, dirLight));
	_lights.push_back(dirLight);


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
	if (_occlusionSphere == NULL)
	{
		configOcclusionSphere();
	}

	// Handle duplicated (name) geoms
	if (doesNameExist(name)) {
		std::cout << "Name already exists: " << name << std::endl;
		return false;
	}

	PointLight *pointLight = new PointLight(name);
	pointLight->setPosition(position);
	pointLight->setColor(color);
	pointLight->setRadius(radius);
	// pointLight->setCastShadow(castShadow);

	setPointLightCastShadow(pointLight, castShadow);

	pointLight->setIntensity(intensity);
	_visualizer->addLight(pointLight);

	_lightsMap.insert(std::make_pair(name, pointLight));
	_lights.push_back(pointLight);

	//if (castShadow && _shadowManager != NULL)
	//{
	//	_shadowManager->addPointLight(pointLight);
	//}
	addPointLightToOcclusionQuery(pointLight);

	++_numLights;

	return true;
}

bool LightManager::setPointLightCastShadow(PointLight *pl, bool tf)
{
	if (_shadowManager == NULL) return false;

	if (tf && !pl->getCastShadow())
	{
		bool res = _shadowManager->addPointLight(pl);
		if (res)
		{
			pl->_castShadow = tf;
		}
		else
		{
			pl->_castShadow = false;
		}
		return res;
	}

	if (!tf && pl->getCastShadow())
	{
		_shadowManager->removePointLight(pl);
		pl->_castShadow = false;
		pl->resetShadowMapProperities();
	}

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

	if (pl != NULL)
	{
		_pointLightOcclusionTestGroup->removeChild(_ocQueryMap[name]);
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


void LightManager::addPointLightToOcclusionQuery(PointLight *pt)
{
	float radius = pt->getRadius();
	osg::MatrixTransform *mt = new osg::MatrixTransform;
	mt->setMatrix(osg::Matrix::scale(osg::Vec3(radius, radius, radius)) * osg::Matrix::translate(pt->getPosition()));
	mt->addChild(_occlusionSphere);
	mt->setUpdateCallback(new PointLightOcclusionNodeCallback(pt));

	osg::OcclusionQueryNode *query = new osg::OcclusionQueryNode;
	query->addChild(mt);
	//TODO: change it back
	//_pointLightOcclusionTestGroup->addChild(mt);
	_pointLightOcclusionTestGroup->addChild(query);

	_ocQueryMap.insert(std::make_pair(pt->getName(), query));
}

bool LightManager::getPointLightOcclusionResult(PointLight *pt)
{
	return _ocQueryMap[pt->getName()]->getPassed(); 
}

// =======================================================================================

PointLightOcclusionNodeCallback::PointLightOcclusionNodeCallback(PointLight *pl)
: _pl(pl)
{
}

void PointLightOcclusionNodeCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	osg::MatrixTransform *mt = static_cast<osg::MatrixTransform *>(node);
	float radius = _pl->getRadius();
	osg::Vec3 pos = _pl->getPosition();
	mt->setMatrix(osg::Matrix::scale(radius, radius, radius) * osg::Matrix::translate(pos));
}


// There is no guarantee the static var is initialized before the constructor of its class or after
osg::ref_ptr<osg::Node> LightManager::_occlusionSphere = NULL;