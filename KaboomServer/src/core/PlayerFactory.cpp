#include "PlayerFactory.h"

#include <core/PositionComponent.h>

#include "PhysicsComponent.h"

PlayerFactory::PlayerFactory(EntityManager *entityManager)
    : entityManager(entityManager) {
}

PlayerFactory::~PlayerFactory() {
}

ServerPlayer *PlayerFactory::createPlayer() const {
    return createPlayer(0.0f, 0.0f, 0.0f);
}

ServerPlayer *PlayerFactory::createPlayer(float x, float y, float z) const {
    const btScalar mass = 1;

    btTransform startTrans = btTransform::getIdentity();
    startTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(startTrans);
    btCollisionShape *collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

    btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, btVector3(0, 0, 0));

    Entity *entity = entityManager->createEntity();
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new PhysicsComponent(rigidBody));

    return static_cast<ServerPlayer *>(entity);
}
