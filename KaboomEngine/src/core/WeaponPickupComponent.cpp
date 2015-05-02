#include "WeaponPickupComponent.h"

WeaponPickupComponent::WeaponPickupComponent(const EntityType &bombType, unsigned int amount)
        : bombType(bombType),
          amount(amount) {
}

const EntityType &WeaponPickupComponent::getBombType() const {
    return bombType;
}

void WeaponPickupComponent::setBombType(const EntityType &bombType) {
    this->bombType = bombType;
}

unsigned int WeaponPickupComponent::getAmount() const {
    return amount;
}

void WeaponPickupComponent::setAmount(unsigned int amount) {
    this->amount = amount;
}
