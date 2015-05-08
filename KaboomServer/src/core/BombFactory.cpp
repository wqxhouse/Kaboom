#include "BombFactory.h"

#include <stdexcept>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <osgDB/XmlParser>

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>
#include <util/XMLLoader.h>

#include "EntityConfigLookup.h"
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
    Entity *entity = entityManager.createEntity(type);

    createBase(entity, x, y, z, vx, vy, vz);

    switch (type) {
        case KABOOM_V2: {
            createKaboomV2(entity);
            break;
        }
        case TIME_BOMB: {
            createTimeBomb(entity);
            break;
        }
    }

    return entity;
}

void BombFactory::createBase(Entity *entity,
        float x,
        float y,
        float z,
        float vx,
        float vy,
        float vz) const {
    const Configuration &config = EntityConfigLookup::instance()[entity->getType()];

    float size = config.getFloat("size");
    float mass = config.getFloat("mass");
    float explosionRadius = config.getFloat("explosion-radius");

    btTransform worldTrans;
    worldTrans.setIdentity();
    worldTrans.setOrigin(btVector3(x, y, z));

    btMotionState *motionState = new btDefaultMotionState(worldTrans);
    btCollisionShape *collisionShape = new btSphereShape(size);

    btVector3 localInertia;
    collisionShape->calculateLocalInertia(mass, localInertia);

    btRigidBody *rigidBody = new btRigidBody(mass, motionState, collisionShape, localInertia);
    rigidBody->setLinearVelocity(btVector3(vx, vy, vz));
    rigidBody->setUserPointer(entity);

    btGhostObject *ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(explosionRadius));
    ghostObject->setWorldTransform(worldTrans);
    ghostObject->setUserPointer(entity);

    entity->attachComponent(new PositionComponent(x, y, z));
    entity->attachComponent(new RotationComponent());
    entity->attachComponent(new PhysicsComponent(rigidBody));
    entity->attachComponent(new TriggerComponent(ghostObject));
}

void BombFactory::createKaboomV2(Entity *entity) const {
    const Configuration &config = EntityConfigLookup::instance()[entity->getType()];

    CollisionHandler *collisionHandler = config.getPointer<CollisionHandler *>("collision-handler");

    entity->attachComponent(new CollisionComponent(collisionHandler));
}

void BombFactory::createTimeBomb(Entity *entity) const {
    const Configuration &config = EntityConfigLookup::instance()[entity->getType()];

    int delay = config.getInt("delay");
    TimerHandler *timerHandler = config.getPointer<TimerHandler *>("timer-handler");

    Timer *timer = new Timer(delay);
    timer->start();
    
    entity->attachComponent(new TimerComponent(timer, timerHandler));
}
