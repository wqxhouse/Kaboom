#include "EntityProcessingSystem.h"

#include "../core/Game.h"

EntityProcessingSystem::EntityProcessingSystem(Game *game)
        : game(game) {
}

void EntityProcessingSystem::process() {
    preprocessEntities(game->getEntityManager().getEntityList());
    processEntities(game->getEntityManager().getEntityList());
}

void EntityProcessingSystem::processEntities(std::vector<Entity *> entities) {
    for (Entity *entity : entities) {
        if (checkEntity(entity)) {
            processEntity(entity);
        }
    }
}
