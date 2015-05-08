#include "CollisionSystem.h"

#include "../components/CollisionComponent.h"
#include "../core/CollisionHandler.h"
#include "../core/Game.h"

CollisionSystem::CollisionSystem(Game *game)
        : game(game) {
}

bool CollisionSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<CollisionComponent>();
}

void CollisionSystem::processEntity(Entity *entity) {
    CollisionComponent *colComp = entity->getComponent<CollisionComponent>();

    if (colComp->isCollided()) {
        colComp->getHandler()->handle(game, entity, colComp->getContactEntities());
    }
}
