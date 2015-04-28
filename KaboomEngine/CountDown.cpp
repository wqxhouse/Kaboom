#include "stdafx.h"
#include "CountDown.h"


CountDown::CountDown(int _maxCoolDown, int _stepAmount, bool _activateOnStart )
	:	maxCoolDown(_maxCoolDown), 
		activated(_activateOnStart),
		stepAmount(_stepAmount),
		duration(_maxCoolDown){
}


CountDown::~CountDown() {
}


bool CountDown::isReady() {
	bool ready = false;
	if (isActivated()) {
		if (duration <= 0) {
			ready = true;
		} else {
			duration -= stepAmount;
		}
	}
	return ready;
}

void CountDown::setActivate(bool _setActivate) {
	activated = _setActivate;
}

bool CountDown::isActivated() {
	return activated;
}


void CountDown::setStepAmount(int _stepAmount, bool resetTimer) {
	stepAmount = _stepAmount;
	//resetTimer();
}

void CountDown::setMaxCoolDown(int _maxCoolDown, bool resetTimer) {
	maxCoolDown = _maxCoolDown;
	//resetTimer();
}

void CountDown::resetTimer() {
	
}

//void CountDown::setActivate(bool)