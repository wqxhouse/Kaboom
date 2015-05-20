#include "TimerSystem.h"

#include <core/Entity.h>
#include <util/Timer.h>

#include "../components/MessageHandlerComponent.h"
#include "../components/TimerComponent.h"
#include "../messaging/MessageHandler.h"
#include "../messaging/TimerExpiredMessage.h"

TimerSystem::TimerSystem(Game *game)
        : EntityProcessingSystem(game) {
}

bool TimerSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<TimerComponent>() &&
            entity->hasComponent<MessageHandlerComponent>();
}

void TimerSystem::processEntity(Entity *entity) {
    auto timerComp = entity->getComponent<TimerComponent>();
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();

    if (timerComp->getTimer()->isExpired()) {
        TimerExpiredMessage message(game, entity, timerComp->getTimer());
        handlerComp->getHandler()->handle(message);
    }
}
