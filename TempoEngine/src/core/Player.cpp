#include "Player.h"

#include "PositionComponent.h"

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
