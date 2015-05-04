#pragma once

#include <unordered_set>

class Entity;
class Game;

class CollisionHandler {
public:
    virtual ~CollisionHandler() {}

    virtual void handle(Game *game, Entity *entity, const std::unordered_set<Entity *> &collidedEntities) = 0;
};
