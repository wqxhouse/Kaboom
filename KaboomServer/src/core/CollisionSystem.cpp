#include "CollisionSystem.h"

#include <btBulletDynamicsCommon.h>

#include "CollisionComponent.h"
#include "ExplosionComponent.h"
#include "Game.h"

CollisionSystem::CollisionSystem(Game *game)
        : EntitySystem(game) {
}

void CollisionSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        CollisionComponent *colComp = entity->getComponent<CollisionComponent>();

        if (colComp == nullptr) {
            continue;
        }

        auto contactEntities = colComp->getContactEntities();

        if (entity->getType() & CAT_BOMB) {
            if (colComp->isCollided()) {
                entity->attachComponent(new ExplosionComponent());
            }
        }
    }
}
