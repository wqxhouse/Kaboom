#include "ExplosionSystem.h"

#include "../components/ExplosionComponent.h"
#include "../components/TriggerComponent.h"
#include "../core/ExplosionHandler.h"
#include "../core/Game.h"

ExplosionSystem::ExplosionSystem(Game *game)
        : game(game) {
}

bool ExplosionSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<ExplosionComponent>() &&
            entity->hasComponent<TriggerComponent>();
}

void ExplosionSystem::processEntity(Entity *entity) {
    ExplosionComponent *expComp = entity->getComponent<ExplosionComponent>();
    TriggerComponent* triggerComp = entity->getComponent<TriggerComponent>();

    expComp->getHandler()->handle(game, entity, triggerComp->getTriggerEntities());
}
