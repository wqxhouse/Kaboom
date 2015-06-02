#pragma once
#include <vector>

class CameraShake
{
public:
	CameraShake(float duration, float frequency, float amplitude);
	float getScreen1DOffset();

private:
	float getAmplitude(float time);
	float getDecay(float time);
	float getNoise(float s);
	void shuffleRandomness();
	void start();
	void update();

	std::vector<float> _samples;
	float _duration;
	float _freq;
	float _amplitude;
	float _startTime;
	float _currTime;
	bool _isShaking;
};