#include "DestroySystem.h"

#include <core/Entity.h>

#include "../components/DestroyComponent.h"
#include "../core/Game.h"

DestroySystem::DestroySystem(Game * game)
        : EntityProcessingSystem(game) {
}

bool DestroySystem::checkEntity(Entity *entity) {
    return entity->hasComponent<DestroyComponent>();
}

void DestroySystem::processEntity(Entity *entity) {
    game->removeEntity(entity);
}
