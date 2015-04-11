#include "PositionComponent.h"

PositionComponent::PositionComponent()
    : PositionComponent(0, 0, 0) {
}

PositionComponent::PositionComponent(float x, float y, float z)
    : x(x), y(y), z(z) {
}

PositionComponent::~PositionComponent() {
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
