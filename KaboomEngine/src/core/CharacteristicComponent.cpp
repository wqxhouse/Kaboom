#include "CharacteristicComponent.h"

CharacteristicComponent::CharacteristicComponent()
    : CharacteristicComponent(UNINITIATED, 0, 0) {
}

CharacteristicComponent::CharacteristicComponent(EntityType type, int feature, int owner)
    : type(type),
    feature(feature),
    owner(owner) {
}

CharacteristicComponent::~CharacteristicComponent() {
}

EntityType CharacteristicComponent::getType() const {
    return type;
}

void CharacteristicComponent::setType(EntityType lype) {
    type = lype;
}

int CharacteristicComponent::getFeature() const {
    return feature;
}

void CharacteristicComponent::setFeature(int i) {
    feature = i;
}

int CharacteristicComponent::getOwner() const {
    return owner;
}

void CharacteristicComponent::setOwner(int o) {
    owner = o;
}
