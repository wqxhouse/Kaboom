#include "PositionComponent.h"

PositionComponent::PositionComponent(float x, float y, float z)
        : x(x),
          y(y),
          z(z) {
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

void PositionComponent::setX(float x) {
    this->x = x;
}

void PositionComponent::setY(float y) {
    this->y = y;
}

void PositionComponent::setZ(float z) {
    this->z = z;
}

void PositionComponent::setPosition(float x, float y, float z) {
    setX(x);
    setY(y);
    setZ(z);
}
