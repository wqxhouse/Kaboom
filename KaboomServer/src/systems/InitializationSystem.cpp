#include "InitializationSystem.h"

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/EntityManager.h>

#include "../components/CollisionComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/TriggerComponent.h"
#include "../core/Game.h"

InitializationSystem::InitializationSystem(Game *game)
        : game(game) {
}

bool InitializationSystem::checkEntity(Entity *entity) {
    return true;
}

void InitializationSystem::processEntity(Entity *entity) {
    // Clear collision results
    CollisionComponent *colComp = entity->getComponent<CollisionComponent>();

    if (colComp != nullptr) {
        colComp->setCollided(false);
        colComp->clearContactEntities();
    }

    // Activate rigid bodies
    PhysicsComponent *physComp = entity->getComponent<PhysicsComponent>();

    if (physComp != nullptr) {
        entity->getComponent<PhysicsComponent>()->getRigidBody()->activate(true);
    }

    // Update trigger position and rotation
    TriggerComponent *triggerComp = entity->getComponent<TriggerComponent>();

    if (triggerComp != nullptr) {
        btTransform worldTrans;

        if (physComp != nullptr) {
            worldTrans = physComp->getRigidBody()->getWorldTransform();
        } else {
            PositionComponent *posComp = entity->getComponent<PositionComponent>();

            if (posComp != nullptr) {
                worldTrans.setOrigin(btVector3(posComp->getX(), posComp->getY(), posComp->getZ()));
            }

            RotationComponent *rotComp = entity->getComponent<RotationComponent>();

            if (rotComp != nullptr) {
                // TODO: set rotation
            }
        }

        triggerComp->getGhostObject()->setWorldTransform(worldTrans);
    }
}

