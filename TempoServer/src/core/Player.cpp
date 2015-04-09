#include "Player.h"

btCollisionShape *Player::collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
const btScalar Player::mass = 1;

Player::Player(PlayerData *data) : data(data) {
    motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(data->x, data->y, data->z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, Player::collisionShape, btVector3(0, 0, 0));
    rigidBody = new btRigidBody(rigidBodyCI);
}

Player::~Player() {
    delete motionState;
    delete rigidBody;
}

void Player::update(btScalar timeStep) {
    btVector3 position = rigidBody->getWorldTransform().getOrigin();
    btVector3 velocity = rigidBody->getLinearVelocity();

    data->x = position.getX();
    data->y = position.getY();
    data->z = position.getZ();
	/*
    data->vx = velocity.getX();
    data->vy = velocity.getY();
    data->vz = velocity.getZ();
	*/
}
