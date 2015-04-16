#include "RotationComponent.h"

RotationComponent::RotationComponent()
    : RotationComponent(0, 0) {
}

RotationComponent::RotationComponent(float yaw, float pitch)
    : yaw(yaw), pitch(pitch) {
}

RotationComponent::~RotationComponent() {
}

float RotationComponent::getYaw() const {
    return yaw;
}

float RotationComponent::getPitch() const {
    return pitch;
}

void RotationComponent::setYaw(float yaw) {
    this->yaw = yaw;
}

void RotationComponent::setPitch(float pitch) {
    this->pitch = pitch;
}

void RotationComponent::setRotation(float yaw, float pitch) {
    setYaw(yaw);
    setPitch(pitch);
}
