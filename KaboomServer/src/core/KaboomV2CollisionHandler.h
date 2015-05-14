#pragma once

#include "CollisionHandler.h"

class KaboomV2CollisionHandler : public CollisionHandler {
public:
    virtual void handle(Game *game, Entity *entity, const std::unordered_set<Entity *> &collidedEntities);
};
