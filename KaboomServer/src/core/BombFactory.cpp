#include "BombFactory.h"

#include <btBulletDynamicsCommon.h>

#include <core/CharacteristicComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>

#include "PhysicsComponent.h"

BombFactory::BombFactory(EntityManager *entityManager)
: entityManager(entityManager) {
}

BombFactory::~BombFactory() {
}

Entity *BombFactory::createBomb() const {
	return createBomb(0.0f, 0.0f, 0.0f);
}

Entity *BombFactory::createBomb(float x, float y, float z) const {
	const btScalar mass = 1;

	btTransform startTrans = btTransform::getIdentity();
	startTrans.setOrigin(btVector3(x, y, z));

	btMotionState *motionState = new btDefaultMotionState(startTrans);
	btCollisionShape *collisionShape = new btSphereShape(0.5);

	btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, btVector3(0, 0, 0));

    Entity *entity = entityManager->createEntity();
    entity->attachComponent(new CharacteristicComponent(BOMB, 0, 0));
	entity->attachComponent(new PositionComponent(x, y, z));
	entity->attachComponent(new PhysicsComponent(rigidBody));

	return entity;
}
