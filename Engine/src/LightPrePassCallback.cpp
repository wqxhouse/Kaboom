#include "stdafx.h" 

#include "LightPrePassCallback.h"
#include <osg/BufferObject>
#include <osg/BufferIndexBinding>
#include <osg/io_utils>

#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Core.h"

LightPrePassCallback::LightPrePassCallback(LightManager *manager)
	: _manager(manager)
{
	_frustumVertices.resize(8);
}

LightPrePassCallback::~LightPrePassCallback()
{
}

void LightPrePassCallback::operator()(osg::StateSet *ss, osg::NodeVisitor *nv)
{
	// 1. light frustum culling 
	std::vector<Light *> visible_lights;
	performLightCulling();

	// 2. update shader input 
	// retrieve ubb
	osg::UniformBufferBinding *ubb = 
		static_cast<osg::UniformBufferBinding *>(ss->getAttribute(osg::StateAttribute::UNIFORMBUFFERBINDING));

	osg::UniformBufferObject* ubo
		= static_cast<osg::UniformBufferObject*>(ubb->getBufferObject());

	osg::FloatArray* array = static_cast<osg::FloatArray*>(ubo->getBufferData(0));

	int uboIndex = 0;
	// TODO: add shadow information
	for (int i = 0; i < _manager->getNumLights(); i++)
	{
		Light *l = _manager->getLight(i);
		//vec3 position;
		//vec3 color;
		//vec3 lookat; // directional light & spot light
		//float radius; // point light

		osg::Vec3 position = l->getPosition();
		osg::Vec3 color = l->getColor();
		osg::Vec3 dirFromLight = osg::Vec3();
		float radius = 0.0f;

		if (l->getLightType() == DIRECTIONAL)
		{
			DirectionalLight *dirLight = l->asDirectionalLight();
			dirFromLight = dirLight->getLightToWorldDirection();
		}
		else if (l->getLightType() == POINT)
		{
			PointLight *ptLight = l->asPointLight();
			radius = ptLight->getRadius();
		}

		// TODO: think a way to do lazy updating 
		// Or is it necessary for UBO?
		(*array)[uboIndex + 0] = (float)position.x();
		(*array)[uboIndex + 1] = (float)position.y();
		(*array)[uboIndex + 2] = (float)position.z();

		(*array)[uboIndex + 3] = (float)color.x();
		(*array)[uboIndex + 4] = (float)color.y();
		(*array)[uboIndex + 5] = (float)color.z();

		(*array)[uboIndex + 6] = (float)dirFromLight.x();
		(*array)[uboIndex + 7] = (float)dirFromLight.y();
		(*array)[uboIndex + 8] = (float)dirFromLight.z();

		(*array)[uboIndex + 9] = radius;

		uboIndex += 10; // for the next light
	}

	array->dirty();
}

std::vector<Light *> LightPrePassCallback::performLightCulling()
{
	std::vector<Light *> visibleLights;
	const Camera &cam = Core::getMainCamera();
	const osg::Matrix projMat = Core::getMainCamera().getClampedProjectionMatrix();
	const osg::Matrix viewMat = Core::getMainCamera().getViewMatrix();

	osg::Matrix inv_vp = osg::Matrix::inverse(viewMat * projMat);

	osg::Polytope frustum;
	frustum.setToUnitFrustum();
	frustum.transformProvidingInverse(inv_vp);

	for (int i = 0; i < _manager->getNumLights(); i++)
	{
		Light *l = _manager->getLight(i);
		bool visible = l->getLightBound().intersectBound(frustum);

		// debug
		if (l->getLightType() == POINT)
		{
			std::cout << visible << std::endl;
		}

		if (visible == true)
		{
			visibleLights.push_back(l);
		}
	}

	return visibleLights;
}