#include "Global.h"
#include "Player.h"

void Player::moveForwardDown() {
	player.movingForward = true;
}

void Player::moveForwardUp() {
    player.movingForward = false;
}

void Player::moveBackwardDown() {
	player.movingBackward = true;
}

void Player::moveBackwardUp() {
    player.movingBackward = false;
}

void Player::moveLeftDown() {
    player.movingLeft = true;
}

void Player::moveLeftUp() {
    player.movingLeft = false;
}

void Player::moveRightDown() {
	player.movingRight = true;
}

void Player::moveRightUp() {
    player.movingRight = false;
}

void Player::jumpDown() {
	player.jumping = true;
}

void Player::jumpUp() {
	player.jumping = false;
}

Player::Player() {
}

Player::~Player() {
}
