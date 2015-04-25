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
	
	// update uniform
	ss->getUniform("u_projInvMat")->set(inv_projMat);

	osg::Vec3 dummy;
	osg::Vec3 eye;
	Core::getMainCamera().getViewMatrix().getLookAt(eye, dummy, dummy);
	ss->getUniform("u_eyePos")->set(eye);
}