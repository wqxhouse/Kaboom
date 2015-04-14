#include "PlayerFactory.h"

#include <core/PositionComponent.h>

#include "PhysicsComponent.h"

PlayerFactory::PlayerFactory(EntityManager *entityManager)
    : entityManager(entityManager) {
}

PlayerFactory::~PlayerFactory() {
}

ServerPlayer *PlayerFactory::createPlayer() const {
    createPlayer(0.0f, 0.0f, 0.0f);
}

ServerPlayer *PlayerFactory::createPlayer(float x, float y, float z) const {
    const btScalar mass = 1;
    btMotionState *motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(x, y, z)));
    btCollisionShape *collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

    btVector3 localInertia;
    collisionShape->calculateLocalInertia(mass, localInertia);

    btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, localInertia);

    Entity *entity = entityManager->createEntity();
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new PhysicsComponent(rigidBody));

    return static_cast<ServerPlayer *>(entity);
}
