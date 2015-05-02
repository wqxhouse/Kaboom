#include "PlayerFactory.h"

#include <btBulletDynamicsCommon.h>

#include <core/BombContainerComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "JetpackComponent.h"

PlayerFactory::PlayerFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *PlayerFactory::createPlayer(float x, float y, float z) const {
    Entity *entity = entityManager.createEntity(PLAYER);

    const btScalar mass = 1;

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
    btCollisionShape *collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

    btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setUserPointer(entity);

    entity->attachComponent(new InputComponent());
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new BombContainerComponent({ { { KABOOM_V2, 10 } } }));
    entity->attachComponent(new JetpackComponent());

    return entity;
}
