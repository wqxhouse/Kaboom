#include "PositionComponent.h"

#include <btBulletDynamicsCommon.h>

#include "PhysicsComponent.h"

PositionComponent::PositionComponent()
    : PositionComponent(0, 0, 0) {
}

PositionComponent::PositionComponent(float x, float y, float z)
    : x(x), y(y), z(z) {
}

PositionComponent::~PositionComponent() {
}

void PositionComponent::update(float timeStep) {
    PhysicsComponent *physicsCom = getEntity()->getComponent<PhysicsComponent>();

    btTransform worldTrans;
    physicsCom->getRigidBody()->getMotionState()->getWorldTransform(worldTrans);

    btVector3 position = worldTrans.getOrigin();

    setX(position.getX());
    setY(position.getY());
    setZ(position.getZ());
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
