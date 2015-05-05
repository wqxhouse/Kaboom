#include "ExplosionSystem.h"

#include "ExplosionHandler.h"
#include "Game.h"
#include "../components/ExplosionComponent.h"
#include "../components/TriggerComponent.h"

ExplosionSystem::ExplosionSystem(Game *game)
        : EntitySystem(game) {
}

void ExplosionSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        ExplosionComponent *expComp = entity->getComponent<ExplosionComponent>();
        TriggerComponent* triggerComp = entity->getComponent<TriggerComponent>();

        if (expComp == nullptr || triggerComp == nullptr) {
            continue;
        }

        expComp->getHandler()->handle(game, entity, triggerComp->getTriggerEntities());
    }
}
