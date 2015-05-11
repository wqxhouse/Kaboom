#include "PlayerStatusComponent.h"

PlayerStatusComponent::PlayerStatusComponent() 
	: isKnockBacked(false),
	  isStaggered(false),
	  isDamaged(false),
	  knockBackTimer(0),
	  staggerTimer(0),
	  damageTimer(0){
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
bool PlayerStatusComponent::checkIsDamaged() {
	if (isDamaged) {
		if (staggerTimer.isExpired()) {
			isDamaged = false;
		}
	}
	return isDamaged;
}

bool PlayerStatusComponent::getIsKnockBacked() const{
	return isKnockBacked;
}
bool PlayerStatusComponent::getIsStaggered() const{
	return isStaggered;
}
bool PlayerStatusComponent::getIsDamaged() const {
	return isDamaged;
}


void PlayerStatusComponent::setIsKnockBacked(bool isKnockBacked) {
	this->isKnockBacked = isKnockBacked;
}
void PlayerStatusComponent::setIsStaggered(bool isStaggered) {
	this->isStaggered = isStaggered;
}
void PlayerStatusComponent::setIsDamaged(bool isDamaged) {
	this->isDamaged = isDamaged;
}


Timer &PlayerStatusComponent::getKnockBackTimer() {
	return knockBackTimer;
}

Timer &PlayerStatusComponent::getStaggerTimer() {
	return staggerTimer;
}

Timer &PlayerStatusComponent::getDamageTimer() {
	return damageTimer;
}