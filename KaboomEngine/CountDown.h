#pragma once
class CountDown {
public:
	CountDown(int _maxCoolDown, int _stepAmount, bool _activateOnStart);
	~CountDown();

	bool isReady();

	bool isActivated();

	void setStepAmount(int, bool);

	void setMaxCoolDown(int, bool);

	void setActivate(bool);

private:
    int duration;
	int maxCoolDown;
	int stepAmount;
	bool activated;

	void resetTimer();
	
};

