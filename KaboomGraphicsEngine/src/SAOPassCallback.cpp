#include "SAOPassCallback.h"
#include <Core.h>

SAOPassCallback::SAOPassCallback(osg::Camera *saoPassCam)
	: _intensity(1.0f), _radius(0.95f),
	_saoPassCam(saoPassCam)
{
	_intensityDivRPow6 = _intensity / pow(_radius, 6.0f);
}

float SAOPassCallback::calcProjScale(float fovy, float viewportHeight)
{
	return viewportHeight / (2.0 * tan(osg::DegreesToRadians(fovy) * 0.5));
}

void SAOPassCallback::operator()(osg::StateSet *ss, osg::NodeVisitor *nv)
{
	Camera &cam = Core::getMainCamera();
	float fovy = cam.getFovY();
	float viewportHeight = _saoPassCam->getViewport()->height();
	float projScale = calcProjScale(fovy, viewportHeight);

	const osg::Vec2 &bufferSize = Core::getRenderResolution();
	osg::Vec2 bufferSizeInv = osg::Vec2(1.0 / bufferSize.x(), 1.0 / bufferSize.y());

	ss->getUniform("u_projScale")->set(projScale);
	ss->getUniform("u_radius")->set(_radius);
	ss->getUniform("u_intensityDivRPow6")->set(_intensityDivRPow6);

	ss->getUniform("u_bufferSizeInv")->set(bufferSizeInv);
}