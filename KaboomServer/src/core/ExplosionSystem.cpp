#include "ExplosionSystem.h"

#include "ExplosionComponent.h"
#include "Game.h"

ExplosionSystem::ExplosionSystem(Game *game)
        : EntitySystem(game) {
}

ExplosionSystem::~ExplosionSystem() {
}

void ExplosionSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (auto entity : entities) {
        if (!entity->hasComponent<ExplosionComponent>()) {
            continue;
        }

        game->getGameServer().sendExplosionEvent(entity);
        game->removeEntity(entity);
    }
}
