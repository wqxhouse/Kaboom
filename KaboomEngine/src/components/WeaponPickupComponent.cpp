#include "WeaponPickupComponent.h"

WeaponPickupComponent::WeaponPickupComponent(const EntityType &bombType, int amount)
        : bombType(bombType),
          amount(amount) {
}

const EntityType &WeaponPickupComponent::getBombType() const {
    return bombType;
}

void WeaponPickupComponent::setBombType(const EntityType &bombType) {
    this->bombType = bombType;
}

int WeaponPickupComponent::getAmount() const {
    return amount;
}

void WeaponPickupComponent::setAmount(int amount) {
    this->amount = amount;
}
