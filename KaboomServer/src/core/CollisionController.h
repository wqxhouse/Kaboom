#pragma once

#include <unordered_set>

class Entity;
class Game;

class CollisionController {
public:
    CollisionController(Entity *entity) : entity(entity) {}
    virtual ~CollisionController() {}

    virtual void onCollision(Game *game, const std::unordered_set<Entity *> &collidedEntities) = 0;

protected:
    Entity *entity;
};
