#pragma once

#include "CollisionHandler.h"

class DefaultCollisionHandler : public CollisionHandler {
public:
    virtual void handle(Game *game, Entity *entity, const std::unordered_set<Entity *> &collidedEntities) {}
};
