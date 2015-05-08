#include "TimerSystem.h"

#include "../components/TimerComponent.h"
#include "../core/TimerHandler.h"
#include "../core/Game.h"

TimerSystem::TimerSystem(Game *game)
        : game(game) {
}

bool TimerSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<TimerComponent>();
}

void TimerSystem::processEntity(Entity *entity) {
    TimerComponent *timerComp = entity->getComponent<TimerComponent>();

    if (timerComp->getTimer()->isExpired()) {
        timerComp->getHandler()->handle(game, entity);
    }
}
