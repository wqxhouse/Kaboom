#pragma once
#include <osg/Matrix>
#include <osg/Vec3>
#include "EffectCompositor.h"

class Camera
{
public:
	Camera() {};
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
		
	// Setters : should only be called from the input manager of the client
	void setEyePositionAndUpdate(const osg::Vec3 &eye);
	void setLookAtAndUpdate(const osg::Vec3 &lookAt);
	void setUpAndUpdate(const osg::Vec3 &up);

	// void setFovAndUpdate(float fov);

private:
	friend class osgFX::EffectCompositor;
	osg::Matrix _viewMatrix;
	osg::Matrix _projMatrix;
	osg::Matrix _clampedProjMatrix;

	osg::Matrix _clampedViewProjMatrix;

	float _nearPlane;
	float _farPlane;

	osg::Vec3 _eye;
	osg::Vec3 _lookat;
	osg::Vec3 _up;

	float _fov;

};
