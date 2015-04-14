#include "ServerPlayer.h"

#include "PhysicsComponent.h"

ServerPlayer::ServerPlayer(unsigned int id)
    : Player(id) {
}

ServerPlayer::~ServerPlayer() {
}

btRigidBody *ServerPlayer::getRigidBody() const {
    return getComponent<PhysicsComponent>()->getRigidBody();
}

float ServerPlayer::getVelocityX() const {
    return getRigidBody()->getLinearVelocity().getX();
}

float ServerPlayer::getVelocityY() const {
    return getRigidBody()->getLinearVelocity().getY();
}

float ServerPlayer::getVelocityZ() const {
    return getRigidBody()->getLinearVelocity().getZ();
}

void ServerPlayer::setVelocityX(float x) {
    btVector3 velocity = getRigidBody()->getLinearVelocity();
    velocity.setX(x);
    getRigidBody()->setLinearVelocity(velocity);
}

void ServerPlayer::setVelocityY(float y) {
    btVector3 velocity = getRigidBody()->getLinearVelocity();
    velocity.setY(y);
    getRigidBody()->setLinearVelocity(velocity);
}

void ServerPlayer::setVelocityZ(float z) {
    btVector3 velocity = getRigidBody()->getLinearVelocity();
    velocity.setZ(z);
    getRigidBody()->setLinearVelocity(velocity);
}
