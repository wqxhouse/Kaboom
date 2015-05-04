#pragma once

#include "CollisionController.h"

class DefaultCollisionController : public CollisionController {
public:
    virtual void onCollision(Game *game, Entity *entity, const std::unordered_set<Entity *> &collidedEntities) {}
};
