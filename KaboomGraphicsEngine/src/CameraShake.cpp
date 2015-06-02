#include "CameraShake.h"
#include <stdlib.h>
#include <time.h>
#include "Core.h"

CameraShake::CameraShake(float duration, float frequency, float amplitude)
: _duration(duration), _freq(frequency), _amplitude(amplitude)
{
	int sampleCount = (duration / 1000.0f) * frequency;
	_samples.resize(sampleCount);

	shuffleRandomness();

	_startTime = -1.0f;
	_currTime = -1.0f;

	_isShaking = false;
}

void CameraShake::start()
{
	shuffleRandomness();
	_startTime = Core::getTimeElaspedMills();
	_currTime = 0.0f;
	_isShaking = true;
}

void CameraShake::update()
{
	if (!_isShaking) return;

	_currTime = Core::getTimeElaspedMills() - _startTime;
	if (_currTime > _duration)
	{
		_isShaking = false;
		_startTime = -1.0f;
		_currTime = -1.0f;
	}
}

void CameraShake::shuffleRandomness()
{
	srand(time(NULL));
	for (int i = 0; i < _samples.size(); i++)
	{
		_samples[i] = (((double)rand() / (RAND_MAX)) + 1);
	}
}

float CameraShake::getAmplitude(float time)
{
	float s = time / 1000.0f * _freq;
	float s0 = floor(s);
	float s1 = s0 + 1;
	float k = getDecay(time);
	return (getNoise(s0) + (s - s0) * (getNoise(s1) - getNoise(s0))) * k;
}

float CameraShake::getDecay(float time)
{
	if (time >= _duration)
	{
		return 0.0f;
	}

	return (_duration - time) / _duration;
}

float CameraShake::getNoise(float s)
{
	if (s >= _samples.size()) return 0.0f;
	return _samples[s];
}

float CameraShake::getScreen1DOffset()
{
	return getAmplitude(_currTime) * _amplitude;
}

//// TODO: currently only calc in view space but not desirable, since pixel shift
//// is subject to proj matrix and buffer resolution;
//osg::Matrix CameraShake::getViewTransform(float time)
//{
//	
//
//	return osg::Matrix();
//}