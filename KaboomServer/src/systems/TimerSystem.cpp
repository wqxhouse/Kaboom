#include "TimerSystem.h"

#include <components/InvulnerabilityComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/Entity.h>
#include <util/Timer.h>

#include "../components/MessageHandlerComponent.h"
#include "../components/TimerComponent.h"

#include "../messaging/MessageHandler.h"
#include "../messaging/TimerExpiredMessage.h"
#include "../messaging/BombDropDespawnMessage.h"

TimerSystem::TimerSystem(Game *game)
        : EntityProcessingSystem(game) {
}

bool TimerSystem::checkEntity(Entity *entity) {
    return (entity->hasComponent<TimerComponent>() || entity->hasComponent<InvulnerabilityComponent>()) &&
            entity->hasComponent<MessageHandlerComponent>();
}

void TimerSystem::processEntity(Entity *entity) {
    auto timerComp = entity->getComponent<TimerComponent>();
    auto invulComp = entity->getComponent<InvulnerabilityComponent>();
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();

    if (timerComp != nullptr) {
        if (entity->hasComponent<WeaponPickupComponent>() && timerComp->getTimer()->isExpired()) {
            BombDropDespawnMessage message(game, entity);
            handlerComp->getHandler()->handle(message);
        } else if (timerComp != nullptr && timerComp->getTimer()->isExpired()) {
            TimerExpiredMessage message(game, entity, timerComp->getTimer());
            handlerComp->getHandler()->handle(message);
        }
    } else if (invulComp != nullptr) {
        if (invulComp->getTimer().isExpired()) {
            entity->detachComponent<InvulnerabilityComponent>();
            delete invulComp;
        }
    }
}
