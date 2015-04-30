#include "LightPassCallback.h"
#include "Core.h"

LightPassCallback::LightPassCallback()
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

	osg::Vec3 dummy;
	osg::Vec3 eye;
	Core::getMainCamera().getViewMatrix().getLookAt(eye, dummy, dummy);
	ss->getUniform("u_eyePos")->set(eye);
}