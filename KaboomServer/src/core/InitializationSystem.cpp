#include "InitializationSystem.h"

#include <core/CharacteristicComponent.h>
#include <core/EntityManager.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>

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

            if (physComp == nullptr) {
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
}
