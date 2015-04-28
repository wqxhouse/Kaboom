#include "CollisionSystem.h"

#include <btBulletDynamicsCommon.h>

#include <core/CharacteristicComponent.h>

#include "Game.h"

CollisionSystem::CollisionSystem(Game *game)
        : EntitySystem(game) {
}

CollisionSystem::~CollisionSystem() {
}

void CollisionSystem::update(float timeStep) {
    const btCollisionDispatcher &dispatcher = game->getWorld().getDispatcher();

    int numManifolds = dispatcher.getNumManifolds();

    for (int i = 0; i < numManifolds; ++i) {
        const btPersistentManifold *contactManifold = dispatcher.getManifoldByIndexInternal(i);

        int numContacts = contactManifold->getNumContacts();

        if (numContacts == 0) {
            continue;
        }

        auto *collisionObjA = static_cast<const btCollisionObject *>(contactManifold->getBody0());
        auto *collisionObjB = static_cast<const btCollisionObject *>(contactManifold->getBody1());

        auto *entityA = static_cast<Entity *>(collisionObjA->getCollisionShape()->getUserPointer());
        auto *entityB = static_cast<Entity *>(collisionObjB->getCollisionShape()->getUserPointer());

        EntityType entityAType = EntityType::UNINITIATED;
        EntityType entityBType = EntityType::UNINITIATED;

        if (entityA != nullptr) {
            entityAType = entityA->getComponent<CharacteristicComponent>()->getType();
        }

        if (entityB != nullptr) {
            entityBType = entityB->getComponent<CharacteristicComponent>()->getType();
        }

        // Getting one contact point should be enough. If not we can change this later.
        const btManifoldPoint &contactPoint = contactManifold->getContactPoint(0);
        const btScalar &contactPointAX = contactPoint.m_positionWorldOnA.getX();
        const btScalar &contactPointAY = contactPoint.m_positionWorldOnA.getY();
        const btScalar &contactPointAZ = contactPoint.m_positionWorldOnA.getZ();
        const btScalar &contactPointBX = contactPoint.m_positionWorldOnB.getX();
        const btScalar &contactPointBY = contactPoint.m_positionWorldOnB.getY();
        const btScalar &contactPointBZ = contactPoint.m_positionWorldOnB.getZ();

        if (entityAType == EntityType::BOMB) {
            game->addBombExplosion(entityA, contactPointAX, contactPointAY, contactPointAZ);
        }

        if (entityBType == EntityType::BOMB) {
            game->addBombExplosion(entityB, contactPointBX, contactPointBY, contactPointBZ);
        }
    }
}
