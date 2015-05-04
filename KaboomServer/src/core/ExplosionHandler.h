#pragma once

#include <unordered_set>

class Entity;
class Game;

class ExplosionHandler {
public:
    virtual ~ExplosionHandler() {}

    virtual void handle(Game *game, Entity *entity, const std::unordered_set<Entity *> &nearbyEntities) = 0;
};
