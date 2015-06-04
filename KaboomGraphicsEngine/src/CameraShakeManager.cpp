#include "CameraShakeManager.h"
#include "Core.h"

CameraShakeManager::CameraShakeManager()
{
	createMildShake();
	createMediumShake();
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

void CameraShakeManager::queueCameraShakeEffect(enum CameraShakeEffects effect)
{
	_shakePriority.push((int)effect);
}

void CameraShakeManager::createViolentShake()
{
	_shakes.push_back(CameraShake2D());
	_shakes.back()._x = new CameraShake(1.0, 60, 16);
	_shakes.back()._y = new CameraShake(1.0, 60, 16);
}

void CameraShakeManager::createMediumShake()
{
	_shakes.push_back(CameraShake2D());
	_shakes.back()._x = new CameraShake(1.0, 30, 4);
	_shakes.back()._y = new CameraShake(1.0, 30, 4);
}

void CameraShakeManager::createMildShake()
{
	_shakes.push_back(CameraShake2D());
	_shakes.back()._x = new CameraShake(0.4, 30, 4);
	_shakes.back()._y = new CameraShake(0.4, 30, 4);
}

osg::Matrix CameraShakeManager::getViewSpaceCamTransform()
{
	int shouldPlay = getShouldPlayShake();
	if (shouldPlay < 0) return osg::Matrix();

	if (isPlaying(shouldPlay))
	{
		stop(shouldPlay);
		start(shouldPlay);
	}

	update(shouldPlay);
	osg::Vec2 pixelOffset = getScreenOffset(shouldPlay);
	osg::Vec2 renderBufSize = Core::getMainCamera().getRenderBufferSize();
	osg::Vec2 renderBufInv = osg::Vec2(1.0f / renderBufSize.x(), 1.0f / renderBufSize.y());
	pixelOffset.x() = (pixelOffset.x() * renderBufInv.x()) * 2.0 - 1.0;
	pixelOffset.y() = (pixelOffset.y() * renderBufInv.y()) * 2.0 - 1.0;
	osg::Vec3 ndc = osg::Vec3(pixelOffset, 0.0f);
	osg::Vec3 vs_offset = 
		ndc * osg::Matrix::inverse(Core::getMainCamera().getClampedProjectionMatrix());
	osg::Vec3 eye = Core::getMainCamera().getEyePosition();
	osg::Vec3 lookAt = Core::getMainCamera().getLookAt();
	osg::Vec3 newEye = eye + vs_offset;
	osg::Vec3 newLookAt = lookAt + vs_offset;

	osg::Matrix newViewMat = osg::Matrix::lookAt(newEye, newLookAt, osg::Vec3(0, 0, 1));
		
	// osg::Matrix updated = vs_offset * viewMat;
	return newViewMat;
}

osg::Vec2 CameraShakeManager::getScreenOffset(int shouldPlay)
{
	float dx = _shakes[shouldPlay]._x->getScreen1DOffset();
	float dy = _shakes[shouldPlay]._y->getScreen1DOffset();

	return osg::Vec2(dx, dy);
}

int CameraShakeManager::getShouldPlayShake()
{
	if (_shakePriority.empty()) return -1;
	int shouldPlay = _shakePriority.top();
	_shakePriority.pop();
	return shouldPlay;
}

void CameraShakeManager::start(int effect)
{
	_shakes[(int)effect]._x->start();
	_shakes[(int)effect]._y->start();
}

void CameraShakeManager::stop(int effect)
{
	_shakes[(int)effect]._x->stop();
	_shakes[(int)effect]._y->stop();
}

void CameraShakeManager::update(int effect)
{
	_shakes[(int)effect]._x->update();
	_shakes[(int)effect]._y->update();
}

bool CameraShakeManager::isPlaying(int effect)
{
	// TODO: make sure
	return _shakes[(int)effect]._x->isShaking() 
		&& _shakes[(int)effect]._y->isShaking();
}