#include "InitializationSystem.h"

#include <core/CharacteristicComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>

#include "CollisionComponent.h"
#include "Game.h"
#include "PhysicsComponent.h"
#include "TriggerComponent.h"

InitializationSystem::InitializationSystem(Game *game)
        : EntitySystem(game) {
}

void InitializationSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        CollisionComponent *colComp = entity->getComponent<CollisionComponent>();

        if (colComp != nullptr) {
            colComp->setCollided(false);
            colComp->clearContactEntities();
        }

        PhysicsComponent *physComp = entity->getComponent<PhysicsComponent>();

        if (physComp == nullptr) {
            continue;
        }

        entity->getComponent<PhysicsComponent>()->getRigidBody()->activate(true);

        const btTransform &worldTrans = physComp->getRigidBody()->getWorldTransform();
        const btVector3 &pos = worldTrans.getOrigin();

        PositionComponent *posCom = entity->getComponent<PositionComponent>();

        if (posCom != nullptr) {
            posCom->setPosition(pos.getX(), pos.getY(), pos.getZ());
        }

        TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

        if (triggerComp != nullptr) {
            triggerComp->getGhostObject()->setWorldTransform(worldTrans);
        }
    }
}
