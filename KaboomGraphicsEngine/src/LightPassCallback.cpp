#include "LightPassCallback.h"
#include "Core.h"
#include "LightManager.h"
#include "ShadowManager.h"

LightPassCallback::LightPassCallback(CubeMapPreFilter *cubemapPrefilter, LightManager *manager)
	: _cubemapPreFilter(cubemapPrefilter), _manager(manager)
{
}

LightPassCallback::~LightPassCallback()
{
}

void LightPassCallback::operator()(osg::StateSet *ss, osg::NodeVisitor *nv)
{
	const osg::Matrix &clampedProjMat = Core::getMainCamera().getClampedProjectionMatrix();
	osg::Matrixf inv_projMat;
	inv_projMat.invert(clampedProjMat);

	// Used in sampling cubemap
	osg::Matrixf inv_viewMat;
	inv_viewMat = inv_viewMat.inverse(Core::getMainCamera().getViewMatrix());
	
	// update uniform
	ss->getUniform("u_viewInvMat")->set(inv_viewMat);
	ss->getUniform("u_projInvMat")->set(inv_projMat);
	ss->getUniform("u_maxLodLevel")->set(_cubemapPreFilter->getCubemapMaxLodLevel());

	ShadowManager *sManager = _manager->getShadowManager();
	int numShadowMap = sManager->getNumShadowMaps();
	std::vector<ShadowMapUniformBlock> data(numShadowMap);

	// retrieve ubb
	osg::UniformBufferBinding *ubb =
		static_cast<osg::UniformBufferBinding *>(ss->getAttribute(osg::StateAttribute::UNIFORMBUFFERBINDING));
	osg::UniformBufferObject* ubo
		= static_cast<osg::UniformBufferObject *>(ubb->getBufferObject());
	osg::FloatArray* array = static_cast<osg::FloatArray*>(ubo->getBufferData(0));

	int uboIndex = 0;

	// TODO: now LightPassConfig.xml has 64 in it, here we have 60
	for (int i = 0; i < sManager->getNumShadowMaps(); i++)
	{
		const osg::Matrix &wvp = sManager->getLightSpaceWVP(i);

		if (!sManager->hasCameraAtIndex(i)) continue;

		// TODO: consider precompute view inv matrix, since it is used every where
		data[i]._vwvp = osg::Matrix::inverse(Core::getMainCamera().getViewMatrix()) * wvp;
		data[i]._atlas_uvcoord = sManager->getAtlasPosUVCoord(i);
		data[i].tex_scale = sManager->getShadowMapScaleWRTAtlas(i);

		// insert ubo
		*(osg::Matrixf *)(&(*array)[uboIndex]) = data[i]._vwvp;
		*(osg::Vec2f *)(&(*array)[uboIndex + 64]) = data[i]._atlas_uvcoord;
		*(float *)(&(*array)[uboIndex + 72]) = data[i].tex_scale;

		uboIndex += 76;
	}

	//osg::Vec3 dummy;
	//osg::Vec3 eye;
	//Core::getMainCamera().getViewMatrix().getLookAt(eye, dummy, dummy);
	//ss->getUniform("u_eyePos")->set(eye);
}