#include "Global.h"
#include "Player.h"

void Player::moveForward() {
	player.movingForward = true;
}

void Player::moveBackward() {
	player.movingBackward = true;
}

void Player::moveLeft() {
	player.movingLeft = true;
}

void Player::moveRight() {
	player.movingRight = true;
}

void Player::stopMoveForward() {
	player.movingForward = false;
}

void Player::stopMoveBackward() {
	player.movingBackward = false;
}

void Player::stopMoveLeft() {
	player.movingLeft = false;
}

void Player::stopMoveRight() {
	player.movingRight = false;
}

void Player::jump() {
	player.jumping = true;
}

void Player::stopJump() {
	player.jumping = false;
}

Player::Player() {
}

Player::~Player() {
}
