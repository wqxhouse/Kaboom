#pragma once

#include "CollisionController.h"

class KaboomV2CollisionController : public CollisionController {
public:
    KaboomV2CollisionController(Entity *entity);

    virtual void onCollision(Game *game, const std::unordered_set<Entity *> &collidedEntities);
};
