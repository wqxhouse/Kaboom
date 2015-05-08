#pragma once

#include <systems/EntitySystem.h>

class Game;

// Spawns the equipped bomb in front of the entity if cooldown is ready.
class FiringSystem : public EntitySystem {
public:
    FiringSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
