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

void Camera::setViewAndUpdate(const osg::Vec3 &eye, const osg::Vec3 &lookAt, const osg::Vec3 &up)
{
	_eye = eye;
	_lookat = lookAt;
	_up = up;

	_viewMatrix.makeLookAt(eye, lookAt, up);
}

void Camera::setFovYAndUpdate(float fovy)
{
	_fovy = fovy;

	// for setters, only modify projMatrix and don't touch the clamped version
	_projMatrix.makePerspective(fovy, _screenSize.x() / _screenSize.y(), _nearPlane, _farPlane);
}

void Camera::setNearAndFarAndUpdate(float near, float far)
{
	_nearPlane = near;
	_farPlane = far;

	_projMatrix.makePerspective(_fovy, _screenSize.x() / _screenSize.y(), _nearPlane, _farPlane);
}

void Camera::setNearAndUpdate(float near)
{
	_nearPlane = near;
	_projMatrix.makePerspective(_fovy, _screenSize.x() / _screenSize.y(), near, _farPlane);
}

void Camera::setFarAndUpdate(float far)
{
	_farPlane = far;
	_projMatrix.makePerspective(_fovy, _screenSize.x() / _screenSize.y(), _nearPlane, far);
}

void Camera::setYawAndPitchAndUpdate(float yaw, float pitch)
{
	_euler.x() = yaw;
	_euler.y() = pitch;


}