#include "WeaponPickupComponent.h"

WeaponPickupComponent::WeaponPickupComponent()
        : WeaponPickupComponent(KABOOM_V2, 1) {
}

WeaponPickupComponent::WeaponPickupComponent(EntityType bombType, unsigned int amount)
        : bombType(bombType),
	      amount(amount){
}

WeaponPickupComponent::~WeaponPickupComponent() {
}

EntityType WeaponPickupComponent::getBombType() const {
    return bombType;
}

void WeaponPickupComponent::setBombType(EntityType bombType) {
	this->bombType = bombType;
}

unsigned int WeaponPickupComponent::getAmount() const {
	return amount;
}

void WeaponPickupComponent::setAmount(unsigned int amount) {
	this->amount = amount;
}

