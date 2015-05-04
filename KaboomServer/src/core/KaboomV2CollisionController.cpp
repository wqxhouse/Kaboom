#include "KaboomV2CollisionController.h"

#include <core/Entity.h>

#include "ExplosionComponent.h"

void KaboomV2CollisionController::onCollision(
        Game *game,
        Entity *entity,
        const std::unordered_set<Entity *> &collidedEntities) {
    entity->attachComponent(new ExplosionComponent());
}
