#include "KaboomV2CollisionHandler.h"

#include <core/Entity.h>

#include "KaboomV2ExplosionHandler.h"
#include "../components/ExplosionComponent.h"

void KaboomV2CollisionHandler::handle(
        Game *game,
        Entity *entity,
        const std::unordered_set<Entity *> &collidedEntities) {
    entity->attachComponent(new ExplosionComponent(new KaboomV2ExplosionHandler));
}
