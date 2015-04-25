#pragma once
#include <osg/Matrix>
#include <osg/Vec3>
#include "EffectCompositor.h"

class Camera
{
public:
	Camera();
	~Camera() {};

	inline const osg::Matrix &getViewMatrix() const
	{
		return _viewMatrix;
	}

	inline const osg::Matrix &getProjectionMatrix() const 
	{
		return _projMatrix;
	}

	inline const osg::Matrix &getClampedProjectionMatrix() const
	{
		return _clampedProjMatrix;
	}

	inline const osg::Matrix &getClampedViewProjectionMatrix() const
	{
		return _clampedViewProjMatrix;
	}

	inline float getNearPlane() const
	{
		return _nearPlane;
	}

	inline float getFarPlane() const
	{
		return _farPlane;
	}

	inline const osg::Vec3 &getEyePosition() const
	{
		return _eye;
	}

	inline const osg::Vec3 &getLookAt() const
	{
		return _lookat;
	}

	inline const osg::Vec3 &getUp() const
	{
		return _up;
	}

	inline float getFovY() const
	{
		return _fovy;
	}
		
	// Setters : should only be called from the input manager of the client
	void setEyePositionAndUpdate(const osg::Vec3 &eye);
	void setLookAtAndUpdate(const osg::Vec3 &lookAt);
	void setUpAndUpdate(const osg::Vec3 &up);
	void setViewAndUpdate(const osg::Vec3 &eye, const osg::Vec3 &lookAt, const osg::Vec3 &up);
	void setFovYAndUpdate(float fov);
	void setNearAndFarAndUpdate(float near, float far);
	void setNearAndUpdate(float near);
	void setFarAndUpdate(float far);
	void setYawAndPitchAndUpdate(float yaw, float pitch);
	void setFpsEyePositionAndUpdate(const osg::Vec3 &eye);

	static osg::Quat eulerToQuat(float yaw, float pitch);

private:
	friend class osgFX::EffectCompositor;
	osg::Matrix _viewMatrix;
	osg::Matrix _projMatrix;
	osg::Matrix _clampedProjMatrix;

	osg::Matrix _clampedViewProjMatrix;

	float _nearPlane;
	float _farPlane;
	float _fovy;

	osg::Vec3 _eye;
	osg::Vec3 _lookat;
	osg::Vec3 _up;
	osg::Vec3 _front;

	osg::Vec2 _screenSize;
	osg::Vec2 _euler;
};
