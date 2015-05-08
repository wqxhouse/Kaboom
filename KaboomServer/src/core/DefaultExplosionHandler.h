#pragma once

#include "ExplosionHandler.h"

class DefaultExplosionHandler : public ExplosionHandler {
public:
    virtual void handle(Game *game, Entity *entity, const std::unordered_set<Entity *> &nearbyEntities);
};
