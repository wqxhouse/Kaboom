#include "CoolDown.h"


CoolDown::CoolDown(clock_t coolDownDuration)
	: coolDownDuration(coolDownDuration),
	startTime(clock()){	
}

CoolDown::~CoolDown() {
}


bool CoolDown::isReady() {
	clock_t timeNow = clock();
	clock_t elapsedTime = timeNow - startTime;
	if (elapsedTime > coolDownDuration) {
		startTime = clock();
		return true;
	}
	return false;
}