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
	std::vector<int> dirShadowLightIds;
	std::vector<int> pointShadowLightIds;
	std::vector<int> shadowMapIndex(6);

	int uboIndex = 0;

	// TODO: add shadow information
	for (int i = 0; i < visible_lights.size(); i++)
	{
		Light *l = visible_lights[i];
	
		// apply viewspace transformation here
		// TODO: fix this, currently this causes flickering
		osg::Vec3 position = l->getPosition() * Core::getMainCamera().getViewMatrix();

		// Here color is multiplied with intensity
		osg::Vec3 color = l->getColor() * l->getIntensity();
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

			if (l->getCastShadow())
			{
				dirShadowLightIds.push_back(i);
			}
			else
			{
				dirLightIds.push_back(i);
			}
		}
		else if (l->getLightType() == POINTLIGHT)
		{
			PointLight *ptLight = l->asPointLight();
			radius = ptLight->getRadius();

			if (l->getCastShadow())
			{
				pointShadowLightIds.push_back(i);
				for (int i = 0; i < 6; i++)
				{
					shadowMapIndex[i] = ptLight->getShadowMapIndex(i);
					// shadowMapIndex[i] = i;
				}
			}
			else
			{
				pointLightIds.push_back(i);
			}
		}

		// TODO: think a way to do lazy updating 
		// Or is it necessary for UBO?
		(*array)[uboIndex + 0] = (float)position.x();
		(*array)[uboIndex + 1] = (float)position.y();
		(*array)[uboIndex + 2] = (float)position.z();
		// radius 
		(*array)[uboIndex + 3] = radius;

		(*array)[uboIndex + 4] = (float)color.x();
		(*array)[uboIndex + 5] = (float)color.y();
		(*array)[uboIndex + 6] = (float)color.z();
		// padding 7
		(*array)[uboIndex + 8] = (float)dirFromLight.x();
		(*array)[uboIndex + 9] = (float)dirFromLight.y();
		(*array)[uboIndex + 10] = (float)dirFromLight.z();
		// padding 11

		// for int array, save int bit pattern in float array
		(*array)[uboIndex + 12] = *(float *)&shadowMapIndex[0]; 
		(*array)[uboIndex + 16] = *(float *)&shadowMapIndex[1]; 
		(*array)[uboIndex + 20] = *(float *)&shadowMapIndex[2]; 
		(*array)[uboIndex + 24] = *(float *)&shadowMapIndex[3]; 
		(*array)[uboIndex + 28] = *(float *)&shadowMapIndex[4]; 
		(*array)[uboIndex + 32] = *(float *)&shadowMapIndex[5]; 

		// debug
		//(*array)[uboIndex + 12] = 1.0f;
		//(*array)[uboIndex + 16] = 2.0f;
		//(*array)[uboIndex + 20] = 3.0f;
		//(*array)[uboIndex + 24] = 4.0f;
		//(*array)[uboIndex + 28] = 5.0f;
		//(*array)[uboIndex + 32] = 6.0f;

		// padding 33 - 36

		uboIndex += 36; // for the next light
	}

	array->dirty();
	// <==== finish ubo 

	// set uniforms
	ss->getUniform("u_countDirectionalLight")->set((int)dirLightIds.size());
	// ss->getUniform("u_countShadowDirectionalLight")->set((int)dirShadowLightIds.size());
	ss->getUniform("u_countPointLight")->set((int)pointLightIds.size());
	ss->getUniform("u_countShadowPointLight")->set((int)pointShadowLightIds.size());

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

	osg::Uniform *pointShadowArray = ss->getUniform("u_arrayShadowPointLight");
	for (int i = 0; i < pointShadowLightIds.size(); i++)
	{
		pointShadowArray->setElement(i, pointShadowLightIds[i]);
	}

	// update projMat 
	ss->getUniform("u_projMat")->set(osg::Matrixf(Core::getMainCamera().getClampedProjectionMatrix()));
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
			// l->setNeedUpdate(false);
		}
	}

	return visibleLights;
}