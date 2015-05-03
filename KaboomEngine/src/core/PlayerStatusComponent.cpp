#include "PlayerStatusComponent.h"

PlayerStatusComponent::PlayerStatusComponent() 
	: isKnockBacked(false),
	  isStaggered(false),
	  knockBackTimer(0),
	  staggerTimer(0){
}

PlayerStatusComponent::~PlayerStatusComponent() {
}


bool PlayerStatusComponent::checkIsKnockBacked() {
	if (isKnockBacked) {
		if (knockBackTimer.isExpired()) {
			isKnockBacked = false;
		}
	}
	return isKnockBacked;
}
bool PlayerStatusComponent::checkIsStaggered() {
	if (isStaggered) {
		if (staggerTimer.isExpired()) {
			isStaggered = false;
		}
	}
	return isStaggered;
}

bool PlayerStatusComponent::getIsKnockBacked() const{
	return isKnockBacked;
}
bool PlayerStatusComponent::getIsStaggered() const{
	return isStaggered;
}

void PlayerStatusComponent::setIsKnockBacked(bool isKnockBacked) {
	this->isKnockBacked = isKnockBacked;
}
void PlayerStatusComponent::setIsStaggered(bool isStaggered) {
	this->isStaggered = isStaggered;
}

Timer &PlayerStatusComponent::getKnockBackTimer() {
	return knockBackTimer;
}

Timer &PlayerStatusComponent::getStaggerTimer() {
	return staggerTimer;
}
