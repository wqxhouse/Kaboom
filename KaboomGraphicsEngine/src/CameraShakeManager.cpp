#include "CameraShakeManager.h"

CameraShakeManager::CameraShakeManager()
{
	createViolentShake();
}

CameraShakeManager::~CameraShakeManager()
{
	for (int i = 0; i < _shakes.size(); i++)
	{
		delete _shakes[i]._x;
		delete _shakes[i]._y;
	}
}

void CameraShakeManager::createViolentShake()
{
	_shakes.push_back(CameraShake2D());
	_shakes.back()._x = new CameraShake(1.0, 60, 16);
	_shakes.back()._y = new CameraShake(1.0, 60, 16);
}

osg::Matrix CameraShakeManager::getViewSpaceCamTransform()
{
	return osg::Matrix();
}

osg::Vec2 CameraShakeManager::getScreenOffset()
{
	return osg::Vec2();
}