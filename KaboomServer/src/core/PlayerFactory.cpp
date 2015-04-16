#include "PlayerFactory.h"

#include <core/PositionComponent.h>

#include "PhysicsComponent.h"

PlayerFactory::PlayerFactory(EntityManager *entityManager)
    : entityManager(entityManager) {
}

PlayerFactory::~PlayerFactory() {
}

ServerPlayer *PlayerFactory::createPlayer(float x, float y, float z) const {
    ServerPlayer *player = new ServerPlayer(entityManager->generateId());
    entityManager->addEntity(player);

    const btScalar mass = 1;

    btTransform startTrans = btTransform::getIdentity();
    startTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(startTrans);
    btCollisionShape *collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

    btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, btVector3(0, 0, 0));

    player->attachComponent(new PositionComponent(x, y, z));
    player->attachComponent(new PhysicsComponent(rigidBody));

    return player;
}
