#pragma once

#include <systems/EntitySystem.h>

class Game;

// Notifies collision handler when a collision occurs.
class CollisionSystem : public EntitySystem {
public:
    CollisionSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
