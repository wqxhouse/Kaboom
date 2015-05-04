#pragma once

#include "CollisionController.h"

class KaboomV2CollisionController : public CollisionController {
public:
    virtual void onCollision(Game *game, Entity *entity, const std::unordered_set<Entity *> &collidedEntities);
};
