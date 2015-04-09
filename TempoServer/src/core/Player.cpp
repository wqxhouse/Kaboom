#include "Player.h"

btCollisionShape *Player::collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

Player::Player(PlayerData *data) : data(data),
	motionState(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(data->x, data->y, data->z)))) {
}

Player::~Player() {
}

void Player::update(btScalar timeStep) {
	btTransform trans;
	motionState->getWorldTransform(trans);

	btVector3 position = trans.getOrigin();

	data->x = position.getX();
	data->y = position.getY();
	data->z = position.getZ();
}
