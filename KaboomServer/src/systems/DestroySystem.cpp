#include "DestroySystem.h"

#include <core/Entity.h>

#include "../components/DestroyComponent.h"
#include "../components/DetonatorComponent.h"
#include "../components/OwnerComponent.h"
#include "../core/Game.h"

DestroySystem::DestroySystem(Game * game)
        : EntityProcessingSystem(game) {
}

bool DestroySystem::checkEntity(Entity *entity) {
    return entity->hasComponent<DestroyComponent>();
}

void DestroySystem::processEntity(Entity *entity) {
    if (entity->getType() == REMOTE_DETONATOR) {
        auto ownerComp = entity->getComponent<OwnerComponent>();

        if (ownerComp != nullptr) {
            auto owner = ownerComp->getEntity();
            auto &bombs = owner->getComponent<DetonatorComponent>()->getBombs();
            bombs.erase(std::find(bombs.begin(), bombs.end(), entity));
        }
    }

    game->removeEntity(entity);
}
