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

	inline const osg::Vec3 &getFront() const
	{
		return _front;
	}

	inline float getFovY() const
	{
		return _fovy;
	}

	inline float getFovX() const
	{
		return fovYToX(_fovy, _screenSize);
	}

	inline osg::Vec2 getScreenSize() const
	{
		return _screenSize;
	}

	inline osg::Vec2 getRenderBufferSize() const
	{
		return _renderBufferSize; 
	}
		
	// Setters : should only be called from the input manager of the client
	void setEyePositionAndUpdate(const osg::Vec3 &eye);
	void setLookAtAndUpdate(const osg::Vec3 &lookAt);
	void setUpAndUpdate(const osg::Vec3 &up);
	void setViewAndUpdate(const osg::Vec3 &eye, const osg::Vec3 &lookAt, const osg::Vec3 &up);
	void setFovYAndUpdate(float fovY);
	void setFovXAndUpdate(float fovX);
	void setNearAndFarAndUpdate(float near, float far);
	void setNearAndUpdate(float near);
	void setFarAndUpdate(float far);
	void setYawAndPitchAndUpdate(float yaw, float pitch);
	void setFpsEyePositionAndUpdate(const osg::Vec3 &eye);

	static osg::Quat eulerToQuat(float yaw, float pitch);
	static float fovXToY(float fovx, const osg::Vec2 screenSize);
	static float fovYToX(float fovy, const osg::Vec2 screenSize);

	osg::Vec3 getScreenCenterCoord(float z);

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
	osg::Vec2 _renderBufferSize;
	osg::Vec2 _euler;
};
