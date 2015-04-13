#include "Player.h"

#include "PositionComponent.h"

Player::Player(Entity *entity)
    : entity(entity) {
}

Player::~Player() {
}

float Player::getX() const {
    return entity->getComponent<PositionComponent>()->getX();
}

float Player::getY() const {
    return entity->getComponent<PositionComponent>()->getY();
}

float Player::getZ() const {
    return entity->getComponent<PositionComponent>()->getZ();
}
