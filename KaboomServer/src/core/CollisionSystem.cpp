#include "CollisionSystem.h"

#include <btBulletDynamicsCommon.h>

#include <core/CharacteristicComponent.h>

#include "CollisionComponent.h"
#include "ExplosionComponent.h"
#include "Game.h"

CollisionSystem::CollisionSystem(Game *game)
        : EntitySystem(game) {
}

CollisionSystem::~CollisionSystem() {
}

void CollisionSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        CollisionComponent *colComp = entity->getComponent<CollisionComponent>();

        if (colComp == nullptr) {
            continue;
        }

        auto contactEntities = colComp->getContactEntities();

        CharacteristicComponent *charComp = entity->getComponent<CharacteristicComponent>();

        switch (charComp->getType()) {
            case EntityType::BOMB: {
                if (colComp->isCollided()) {
                    entity->attachComponent(new ExplosionComponent());
                }
                break;
            }
        }
    }
}
