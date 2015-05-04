#pragma once

#include <unordered_set>

class Entity;
class Game;

class CollisionController {
public:
    virtual ~CollisionController() {}

    virtual void onCollision(Game *game, Entity *entity, const std::unordered_set<Entity *> &collidedEntities) = 0;
};
