#include "CollisionSystem.h"

#include "../components/CollisionComponent.h"
#include "../core/CollisionHandler.h"
#include "../core/Game.h"

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

        if (colComp->isCollided()) {
            colComp->getHandler()->handle(game, entity, colComp->getContactEntities());
        }
    }
}
