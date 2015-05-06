#include "TimerSystem.h"

#include "../components/TimerComponent.h"
#include "../core/TimerHandler.h"
#include "../core/Game.h"

TimerSystem::TimerSystem(Game *game)
    : EntitySystem(game) {
}

void TimerSystem::update(float timeStep) {
    auto entities = game->getEntityManager().getEntityList();

    for (Entity *entity : entities) {
        TimerComponent *timerComp = entity->getComponent<TimerComponent>();

        if (timerComp == nullptr) {
            continue;
        }

        if (timerComp->getTimer()->isExpired()) {
            timerComp->getHandler()->handle(game, entity);
        }
    }
}
