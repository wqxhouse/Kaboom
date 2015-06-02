#include "World.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <core/Entity.h>
#include <core/EntityType.h>
#include <util/ConfigSettings.h>

#include "WorldLoader.h"
#include "../components/CollisionComponent.h"
#include "../components/TriggerComponent.h"
#include "../components/JumpComponent.h"

void onTickCallback(btDynamicsWorld *world, btScalar timeStep) {
    World *w = static_cast<World *>(world->getWorldUserInfo());
    w->onTick(timeStep);
}

World::World(ConfigSettings* configSettings)
        : configSettings(configSettings),
          dispatcher(&collisionConfiguration),
          world(&dispatcher, &broadphase, &solver, &collisionConfiguration) {
    setGravity(9.8f);
    world.setInternalTickCallback(onTickCallback, this);
}

void World::load(const std::string &mapFilename, const std::string &entitiesFilename) {
    std::string mediaPath;
    configSettings->getValue(ConfigSettings::str_mediaFilePath, mediaPath);

    WorldLoader loader(*this);
    loader.loadMap(mapFilename, mediaPath);
    loader.loadEntities(entitiesFilename);
}

void World::stepSimulation(float timeStep, int maxSubSteps) {
    world.stepSimulation(timeStep, maxSubSteps, timeStep / maxSubSteps);
}

void World::addRigidBody(btRigidBody *rigidBody) {
    world.addRigidBody(rigidBody);
}

void World::removeRigidBody(btRigidBody *rigidBody) {
    world.removeRigidBody(rigidBody);
}

void World::addTrigger(btGhostObject *ghostObject) {
    ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    world.addCollisionObject(
            ghostObject,
            btBroadphaseProxy::SensorTrigger,
            btBroadphaseProxy::AllFilter ^ btBroadphaseProxy::SensorTrigger ^ btBroadphaseProxy::StaticFilter);
}

void World::removeTrigger(btGhostObject *ghostObject) {
    world.removeCollisionObject(ghostObject);
}

void World::setGravity(float gravity) {
    world.setGravity(btVector3(0, 0, -gravity));
}

void World::onTick(btScalar timeStep) {
    int numManifolds = dispatcher.getNumManifolds();

    for (int i = 0; i < numManifolds; ++i) {
        const btPersistentManifold *manifold = dispatcher.getManifoldByIndexInternal(i);

        int numContacts = manifold->getNumContacts();

        if (numContacts == 0) {
            continue;
        }

        const btCollisionObject *collisionObjA = static_cast<const btCollisionObject *>(manifold->getBody0());
        const btCollisionObject *collisionObjB = static_cast<const btCollisionObject *>(manifold->getBody1());

        btManifoldPoint contactPoint = manifold->getContactPoint(0);

        Entity *entityA = static_cast<Entity *>(collisionObjA->getUserPointer());
        Entity *entityB = static_cast<Entity *>(collisionObjB->getUserPointer());

        if (entityA == entityB) {
            continue;
        }

        if (collisionObjA->hasContactResponse() && collisionObjB->hasContactResponse()) {
            handleCollision(entityA, entityB, contactPoint);
            handleCollision(entityB, entityA, contactPoint);
        } else {
            handleTrigger(entityA, entityB, contactPoint);
            handleTrigger(entityB, entityA, contactPoint);
        }
    }
}

void World::addStaticPlane(btVector3 origin, btVector3 normal) {
    addStaticPlane(origin, normal, btQuaternion::getIdentity());
}

void World::addStaticPlane(btVector3 origin, btVector3 normal, btQuaternion rotation) {
    btTransform startTrans;
    startTrans.setIdentity();
    startTrans.setOrigin(origin);
    startTrans.setRotation(rotation);

    btCollisionShape *groundShape = new btStaticPlaneShape(normal, 0);
    btDefaultMotionState *groundMotionState = new btDefaultMotionState(startTrans);

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);

    addRigidBody(groundRigidBody);
}

void World::handleCollision(Entity *entityA, Entity *entityB, const btManifoldPoint &contactPoint) const {
    if (entityA != nullptr) {
        CollisionComponent *colComp = entityA->getComponent<CollisionComponent>();

        if (colComp != nullptr) {
            colComp->setCollided(true);

            if (entityB != nullptr) {
                colComp->addContactEntity(entityB);
            }
        }

        // only test if one of them is a character
        if (entityA->hasComponent<JumpComponent>()) {
            bool collideGround = isCollidingGround(contactPoint);

            JumpComponent* jumpComp = entityA->getComponent<JumpComponent>();
            if (collideGround) {
                jumpComp->setJumping(false);

            }
        }
    }
}

void World::handleTrigger(Entity *entityA, Entity *entityB, const btManifoldPoint &contactPoint) const {
    if (entityA != nullptr) {
        auto *triggerComp = entityA->getComponent<TriggerComponent>();

        if (triggerComp != nullptr && entityB != nullptr) {
            triggerComp->addTriggerEntity(entityB);
        }
    }
}

bool World::isCollidingGround(const btManifoldPoint &contactPoint) const {
    btVector3 normal = contactPoint.m_normalWorldOnB;
    btVector3 up = btVector3(0, 0, 1);
    double dot = normal.dot(up);

    // allow for error
    return dot > 0.05 ? true : false;
}
