#include "ExplosionSystem.h"

#include <core/Entity.h>

#include "../components/MessageHandlerComponent.h"
#include "../components/TriggerComponent.h"
#include "../messaging/ExplosionMessage.h"

class ExplosionComponent;

ExplosionSystem::ExplosionSystem(Game *game)
        : game(game) {
}

bool ExplosionSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<ExplosionComponent>() &&
            entity->hasComponent<TriggerComponent>() &&
            entity->hasComponent<MessageHandlerComponent>();
}

void ExplosionSystem::processEntity(Entity *entity) {
    auto triggerComp = entity->getComponent<TriggerComponent>();
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();

    ExplosionMessage message(game, entity, triggerComp->getTriggerEntities());
    handlerComp->getHandler()->handle(message);
}
