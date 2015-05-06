#include "TimeBombTimerHandler.h"

#include <core/Entity.h>

#include "DefaultExplosionHandler.h"
#include "../components/ExplosionComponent.h"

void TimeBombTimerHandler::handle(Game *game, Entity *entity) {
    entity->attachComponent(new ExplosionComponent(new DefaultExplosionHandler()));
}
