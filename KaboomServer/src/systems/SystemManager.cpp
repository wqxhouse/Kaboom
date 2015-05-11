#include "SystemManager.h"

#include <systems/EntitySystem.h>

#include "../core/Game.h"

void SystemManager::addSystem(EntitySystem *system) {
    systems.push_back(system);
}

void SystemManager::processSystems(Game *game) {
    for (EntitySystem *system : systems) {
        system->preprocessEntities(game->getEntityManager().getEntityList());
        system->processEntities(game->getEntityManager().getEntityList());
    }
}
