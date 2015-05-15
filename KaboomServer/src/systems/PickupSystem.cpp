#include "PickupSystem.h"

#include <core/Entity.h>

#include "../components/MessageHandlerComponent.h"
#include "../components/TriggerComponent.h"
#include "../messaging/MessageHandler.h"
#include "../messaging/PickupMessage.h"

class WeaponPickupComponent;

PickupSystem::PickupSystem(Game *game)
        : game(game) {
}

bool PickupSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<WeaponPickupComponent>() &&
            entity->hasComponent<TriggerComponent>() &&
            entity->hasComponent<MessageHandlerComponent>();
}

void PickupSystem::processEntity(Entity *entity) {
    auto triggerComp = entity->getComponent<TriggerComponent>();
    auto handlerComop = entity->getComponent<MessageHandlerComponent>();

    auto triggerEntities = triggerComp->getTriggerEntities();

    PickupMessage message(game, entity, triggerEntities);
    handlerComop->getHandler()->handle(message);
}
