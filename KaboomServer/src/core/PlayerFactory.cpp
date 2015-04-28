#include "PlayerFactory.h"

#include <btBulletDynamicsCommon.h>

#include <core/BombContainerComponent.h>
#include <core/CharacteristicComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

#include "InputComponent.h"
#include "PhysicsComponent.h"
#include "JetpackComponent.h"

PlayerFactory::PlayerFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

PlayerFactory::~PlayerFactory() {
}

Entity *PlayerFactory::createPlayer() const {
    return createPlayer(0.0f, 0.0f, 0.0f);
}

Entity *PlayerFactory::createPlayer(float x, float y, float z) const {
    Entity *entity = entityManager.createEntity();

    const btScalar mass = 1;

    btTransform startTrans = btTransform::getIdentity();
    startTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(startTrans);
    btCollisionShape *collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    collisionShape->setUserPointer(entity);

    btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, btVector3(0, 0, 0));

    entity->attachComponent(new CharacteristicComponent(PLAYER, 0, 0));
    entity->attachComponent(new InputComponent());
    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new BombContainerComponent());
	entity->attachComponent(new JetpackComponent());

    return entity;
}
