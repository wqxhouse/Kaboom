#include "WeaponPickupComponent.h"

WeaponPickupComponent::WeaponPickupComponent()
	: WeaponPickupComponent(BombType::CRACKER_BOMB, 1) {
}

WeaponPickupComponent::WeaponPickupComponent(BombType bombType, unsigned int amount)
    : bombType(bombType),
	  amount(amount){
}

WeaponPickupComponent::~WeaponPickupComponent() {
}

BombType WeaponPickupComponent::getBombType() const {
    return bombType;
}

void WeaponPickupComponent::setBombType(BombType bombType) {
	this->bombType = bombType;
}

unsigned int WeaponPickupComponent::getAmount() const {
	return amount;
}

void WeaponPickupComponent::setAmount(unsigned int amount) {
	this->amount = amount;
}

