#include "DestroySystem.h"

#include "../components/DestroyComponent.h"

DestroySystem::DestroySystem(Game * game)
        : game(game) {
}

bool DestroySystem::checkEntity(Entity *entity) {
    return entity->hasComponent<DestroyComponent>();
}

void DestroySystem::processEntity(Entity *entity) {
    game->removeEntity(entity);
}
