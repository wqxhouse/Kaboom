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
	std::vector<Light *> visible_lights = performLightCulling();

	// 2. update shader input 
	// retrieve ubb
	osg::UniformBufferBinding *ubb = 
		static_cast<osg::UniformBufferBinding *>(ss->getAttribute(osg::StateAttribute::UNIFORMBUFFERBINDING));

	osg::UniformBufferObject* ubo
		= static_cast<osg::UniformBufferObject*>(ubb->getBufferObject());

	osg::FloatArray* array = static_cast<osg::FloatArray*>(ubo->getBufferData(0));
	
	std::vector<int> dirLightIds;
	std::vector<int> pointLightIds;

	int uboIndex = 0;
	//std::cout << Core::getMainCamera().getNearPlane() << std::endl;
	//std::cout << Core::getMainCamera().getFarPlane() << std::endl;

	// TODO: add shadow information
	for (int i = 0; i < visible_lights.size(); i++)
	{
		Light *l = visible_lights[i];
		//vec3 position;
		//vec3 color;
		//vec3 lookat; // directional light & spot light
		//float radius; // point light

		// apply viewspace transformation here
		// TODO: fix this, currently this causes flickering
		osg::Vec3 position = l->getPosition() * Core::getMainCamera().getViewMatrix();
		// std::cout << position << std::endl;

		// osg::Vec3 position = l->getPosition();
		osg::Vec3 color = l->getColor();
		osg::Vec3 dirFromLight = osg::Vec3();
		float radius = 0.0f;

		if (l->getLightType() == DIRECTIONAL)
		{
			DirectionalLight *dirLight = l->asDirectionalLight();
			// dirFromLight = dirLight->getLightToWorldDirection();
			osg::Vec4 dir = osg::Vec4(dirLight->getLightToWorldDirection(), 0);
			dir = dir * Core::getMainCamera().getViewMatrix();
			dirFromLight.x() = dir.x();
			dirFromLight.y() = dir.y();
			dirFromLight.z() = dir.z();

			// std::cout << Core::getMainCamera().getViewMatrix() << std::endl;
			// std::cout << dir << std::endl;

			dirLightIds.push_back(i);
		}
		else if (l->getLightType() == POINTLIGHT)
		{
			PointLight *ptLight = l->asPointLight();
			radius = ptLight->getRadius();

			pointLightIds.push_back(i);
		}

		// TODO: think a way to do lazy updating 
		// Or is it necessary for UBO?
		(*array)[uboIndex + 0] = (float)position.x();
		(*array)[uboIndex + 1] = (float)position.y();
		(*array)[uboIndex + 2] = (float)position.z();
		// padding 3
		(*array)[uboIndex + 4] = (float)color.x();
		(*array)[uboIndex + 5] = (float)color.y();
		(*array)[uboIndex + 6] = (float)color.z();
		// padding 7
		(*array)[uboIndex + 8] = (float)dirFromLight.x();
		(*array)[uboIndex + 9] = (float)dirFromLight.y();
		(*array)[uboIndex + 10] = (float)dirFromLight.z();

		(*array)[uboIndex + 11] = radius;

		uboIndex += 12; // for the next light
	}

	array->dirty();
	// <==== finish ubo 

	// set uniforms
	ss->getUniform("u_countDirectionalLight")->set((int)dirLightIds.size());
	ss->getUniform("u_countPointLight")->set((int)pointLightIds.size());

	// set dirLight array
	osg::Uniform *dirArray = ss->getUniform("u_arrayDirectionalLight");
	for (int i = 0; i < dirLightIds.size(); i++)
	{
		dirArray->setElement(i, dirLightIds[i]);
	}

	osg::Uniform *pointArray = ss->getUniform("u_arrayPointLight");
	for (int i = 0; i < pointLightIds.size(); i++)
	{
		pointArray->setElement(i, pointLightIds[i]);
	}

	// update mvp 
	ss->getUniform("u_viewMat")->set(osg::Matrixf(Core::getMainCamera().getViewMatrix()));
	ss->getUniform("u_projMat")->set(osg::Matrixf(Core::getMainCamera().getClampedProjectionMatrix()));
	ss->getUniform("u_viewProjMat")->set(osg::Matrixf(Core::getMainCamera().getClampedViewProjectionMatrix()));

	//ss->getUniform("u_viewProjMat")->set(osg::Matrixf(Core::getMainCamera().getViewMatrix()) * osg::Matrixf(Core::getMainCamera().getProjectionMatrix()));
}

std::vector<Light *> LightPrePassCallback::performLightCulling()
{
	std::vector<Light *> visibleLights;
	const Camera &cam = Core::getMainCamera();
	const osg::Matrix projMat = Core::getMainCamera().getClampedProjectionMatrix();
	const osg::Matrix viewMat = Core::getMainCamera().getViewMatrix();

	osg::Matrix inv_vp = osg::Matrix::inverse(viewMat * projMat);

	//osg::Polytope frustum;
	//frustum.setToUnitFrustum();
	//frustum.transformProvidingInverse(inv_vp);

	// debug 
	/*osg::Matrix dview;
	dview.makeLookAt(osg::Vec3(0, -100, 0), osg::Vec3(0, 0, 0), osg::Vec3(0, 0, 1));

	osg::Matrix dproj;
	dproj.makeFrustum(-1, 1, -1, 1, 0.1, 1000);

	osg::Matrix dmvp = dview * dproj;
	osg::Matrix invMvp;
	invMvp.invert(dmvp);*/

	osg::Polytope frustum;
	frustum.setToUnitFrustum();
	frustum.transform(inv_vp); 

	for (int i = 0; i < _manager->getNumLights(); i++)
	{
		Light *l = _manager->getLight(i);

		if (i >= 256)
		{
			OSG_WARN << "reaching maximum visible lights, skipping " << l->getName() << std::endl;
			continue;
		}

		bool visible = l->getLightBound().intersectBound(frustum);

		if (visible == true)
		{
			visibleLights.push_back(l);
		}
		else
		{
			// TODO: output light ... culled, use custom logging later
			OSG_WARN << "light " << l->getName() << " frustum culled." << std::endl;
		}
	}

	return visibleLights;
}