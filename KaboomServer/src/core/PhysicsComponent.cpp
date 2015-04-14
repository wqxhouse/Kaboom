#include "PhysicsComponent.h"

PhysicsComponent::PhysicsComponent(btRigidBody *rigidBody)
    : rigidBody(rigidBody) {
}

PhysicsComponent::~PhysicsComponent() {
}

btRigidBody *PhysicsComponent::getRigidBody() const {
    return rigidBody;
}

void PhysicsComponent::setRigidBody(btRigidBody *rigidBody) {
    this->rigidBody = rigidBody;
}
