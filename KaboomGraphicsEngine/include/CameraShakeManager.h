#pragma once
#include "CameraShake.h"
#include <queue>

class CameraShakeManager
{
public:
	enum CameraShakeEffects
	{
		// INCREASING WEIGHT
		MILD, 
		MEDIUM,
		VIOLENT, 
	};

	CameraShakeManager();
	~CameraShakeManager();
	void queueCameraShakeEffect(enum CameraShakeEffects effect);

	osg::Matrix getViewSpaceCamTransform();

private:
	struct CameraShake2D
	{
		CameraShake *_x;
		CameraShake *_y;
	};

	osg::Vec2 getScreenOffset(int shouldPlay);
	int getShouldPlayShake();
	void start(int effect);
	void stop(int effect);
	void update(int effect);
	bool isPlaying(int effect);

	void createViolentShake();
	void createMildShake();
	void createMediumShake();

	std::vector<CameraShake2D> _shakes;
	std::priority_queue<int> _shakePriority;
};