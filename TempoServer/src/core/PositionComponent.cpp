#include "PositionComponent.h"

PositionComponent::PositionComponent()
    : x(0), y(0), z(0) {
}

PositionComponent::PositionComponent(float x, float y, float z)
    : x(x), y(y), z(z) {
}

PositionComponent::~PositionComponent() {
}

ComponentType PositionComponent::getType() const {
    return POSITION;
}

float PositionComponent::getX() const {
    return x;
}

float PositionComponent::getY() const {
    return y;
}

float PositionComponent::getZ() const {
    return z;
}

void PositionComponent::setX(const float &x) {
    this->x = x;
}

void PositionComponent::setY(const float &y) {
    this->y = y;
}

void PositionComponent::setZ(const float &z) {
    this->z = z;
}
