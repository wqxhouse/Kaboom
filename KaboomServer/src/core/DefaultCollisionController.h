#pragma once

#include "CollisionController.h"

class DefaultCollisionController : public CollisionController {
public:
    DefaultCollisionController(Entity *entity) : CollisionController(entity) {}

    virtual void onCollision(Game *game, const std::unordered_set<Entity *> &collidedEntities) {}
};
