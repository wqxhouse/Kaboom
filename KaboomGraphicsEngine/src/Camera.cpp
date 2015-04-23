#include "Camera.h"

void Camera::setEyePositionAndUpdate(const osg::Vec3 &eye)
{
	_eye = eye;
	_viewMatrix.makeLookAt(eye, _lookat, _up);
}

void Camera::setLookAtAndUpdate(const osg::Vec3 &lookAt)
{
	_lookat = lookAt;
	_viewMatrix.makeLookAt(_eye, lookAt, _up);
}

void Camera::setUpAndUpdate(const osg::Vec3 &up)
{
	_up = up;
	_viewMatrix.makeLookAt(_eye, _lookat, up);
}

void Camera::setFovYAndUpdate(float fovy)
{
	_fovy = fovy;

}