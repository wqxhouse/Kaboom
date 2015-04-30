#include "JetpackComponent.h"

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



JetpackComponent::~JetpackComponent() {
}

