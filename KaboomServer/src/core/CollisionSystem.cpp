#include "CollisionSystem.h"

#include "CollisionComponent.h"
#include "CollisionController.h"
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

        if (colComp->isCollided()) {
            colComp->getController()->onCollision(game, colComp->getContactEntities());
        }
    }
}
