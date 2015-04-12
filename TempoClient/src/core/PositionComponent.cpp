#include "PositionComponent.h"

PositionComponent::PositionComponent()
    : PositionComponent(0, 0, 0) {
}

PositionComponent::PositionComponent(float x, float y, float z)
    : x(x), y(y), z(z) {
}

PositionComponent::~PositionComponent() {
}

void PositionComponent::update(const GameStateData &gameState) {
    if (entity->getId() == 0) {
        setX(gameState.x1);
        setY(gameState.y1);
        setZ(gameState.z1);
    } else if (entity->getId() == 1) {
        setX(gameState.x2);
        setY(gameState.y2);
        setZ(gameState.z2);
    }
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
