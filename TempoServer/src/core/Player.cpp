#include "Player.h"

btCollisionShape *Player::collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

Player::Player(PlayerData *data) : data(data) {
}

Player::~Player() {
}

void Player::update(btScalar timeStep) {

}
