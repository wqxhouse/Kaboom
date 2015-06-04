#pragma once
#include <vector>

class CameraShake
{
public:
	CameraShake(float duration, float frequency, float amplitude);
	float getScreen1DOffset();

	void start();
	void stop();
	void update();

	inline bool isShaking()
	{
		return _isShaking;
	}

private:
	float getAmplitude(float time);
	float getDecay(float time);
	float getNoise(float s);
	void shuffleRandomness();

	std::vector<float> _samples;
	float _duration;
	float _freq;
	float _amplitude;
	float _startTime;
	float _currTime;
	bool _isShaking;
};