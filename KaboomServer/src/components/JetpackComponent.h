#pragma once

#include <components/Component.h>

class JetpackComponent : public Component {
public:
    JetpackComponent(
            int fuel = 0,
            int capacity = 0,
            int refuelRate = 0,
            int refuelTime = 0)
            : fuel(fuel),
              capacity(capacity),
              refuelRate(refuelRate),
              refuelTime(refuelTime) {
    }

    inline int getFuel() const {
        return fuel;
    }

    inline int getCapacity() const {
        return capacity;
    }

    inline int getRefuelRate() const {
        return refuelRate;
    }

    inline int getRefuelTime() const {
        return refuelTime;
    }

private:
    int fuel;
    int capacity;
    int refuelRate;
    int refuelTime;
};

/*
#include <ctime>

#define REFUELRATE 25
#define TANKSIZE 100
#define REFUELTIME 1000

JetpackComponent::JetpackComponent(){
	jumpsLeft = TANKSIZE;
	beginTime = clock();
}

bool JetpackComponent::activateJetpack(){
	if (jumpsLeft >= 1){
		beginTime = clock();
		jumpsLeft--;
		return true;
	}
	return false;
}
void JetpackComponent::refillJetpack(){
	
	if (jumpsLeft >= TANKSIZE){
		return;
	}
	clock_t endTime = clock();
	if (endTime-beginTime<REFUELTIME){
		return;
	}
	beginTime = clock();
	if (100 - jumpsLeft>REFUELRATE){
		jumpsLeft = TANKSIZE - jumpsLeft;
	}
	jumpsLeft+=REFUELRATE;
}
*/