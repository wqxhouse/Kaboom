#pragma once
#include <time.h> 

class CoolDown {
public:
	CoolDown(clock_t coolDownDuration);

	bool isReady();

private:
	clock_t startTime;
	clock_t coolDownDuration;
};

