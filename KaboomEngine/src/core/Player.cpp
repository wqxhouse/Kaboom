#include "Player.h"

#include "PositionComponent.h"
#include "RotationComponent.h"

Player::Player(unsigned int id)
    : Entity(id) {
}

Player::~Player() {
}

float Player::getX() const {
    return getComponent<PositionComponent>()->getX();
}

float Player::getY() const {
    return getComponent<PositionComponent>()->getY();
}

float Player::getZ() const {
    return getComponent<PositionComponent>()->getZ();
}

void Player::setX(float x) {
    getComponent<PositionComponent>()->setX(x);
}

void Player::setY(float y) {
    getComponent<PositionComponent>()->setY(y);
}

void Player::setZ(float z) {
    getComponent<PositionComponent>()->setZ(z);
}

void Player::setPosition(float x, float y, float z) {
    getComponent<PositionComponent>()->setPosition(x, y, z);
}

float Player::getYaw() const {
    return getComponent<RotationComponent>()->getYaw();
}

float Player::getPitch() const {
    return getComponent<RotationComponent>()->getPitch();
}

void Player::setYaw(float yaw) {
    getComponent<RotationComponent>()->setYaw(yaw);
}

void Player::setPitch(float pitch) {
    getComponent<RotationComponent>()->setPitch(pitch);
}

void Player::setRotation(float yaw, float pitch) {
    getComponent<RotationComponent>()->setRotation(yaw, pitch);
}
