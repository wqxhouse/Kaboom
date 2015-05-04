#include "KaboomV2CollisionController.h"

#include <core/Entity.h>

#include "ExplosionComponent.h"

KaboomV2CollisionController::KaboomV2CollisionController(Entity *entity)
        : CollisionController(entity) {
}

void KaboomV2CollisionController::onCollision(
        Game *game,
        const std::unordered_set<Entity *> &collidedEntities) {
    entity->attachComponent(new ExplosionComponent());
}
