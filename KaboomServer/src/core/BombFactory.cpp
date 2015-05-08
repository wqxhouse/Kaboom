#include "BombFactory.h"

#include <stdexcept>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <osgDB/XmlParser>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>
#include <util/XMLLoader.h>

#include "BombDataLookup.h"
#include "KaboomV2CollisionHandler.h"
#include "TimeBombTimerHandler.h"
#include "../components/CollisionComponent.h"
#include "../components/ExplosionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/TimerComponent.h"
#include "../components/TriggerComponent.h"

BombFactory::BombFactory(EntityManager &entityManager)
        : entityManager(entityManager) {
}

Entity *BombFactory::createBomb(
        const EntityType &type,
        float x,
        float y,
        float z,
        float vx,
        float vy,
        float vz) const {
    const Configuration &bombConfig = BombDataLookup::instance()[type];

    Entity *entity = entityManager.createEntity(type);

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
    btCollisionShape *collisionShape = new btSphereShape(bombConfig.getFloat("size"));

    btRigidBody *rigidBody = new btRigidBody(bombConfig.getFloat("mass"), motionState, collisionShape, btVector3(0, 0, 0));
    rigidBody->setLinearVelocity(btVector3(vx, vy, vz));
    rigidBody->setUserPointer(entity);

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(bombConfig.getFloat("explosion-radius")));
    ghostObject->setWorldTransform(worldTrans);
    ghostObject->setUserPointer(entity);

    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new TriggerComponent(ghostObject));

    if (type == KABOOM_V2) {
        entity->attachComponent(new CollisionComponent(bombConfig.getPointer<CollisionHandler *>("collision-handler")));
    }

    if (type == TIME_BOMB) {
        Timer *timer = new Timer(bombConfig.getInt("delay"));
        timer->start();
        entity->attachComponent(new TimerComponent(timer, new TimeBombTimerHandler()));
    }

    return entity;
}
