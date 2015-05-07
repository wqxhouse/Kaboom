#pragma once

#include <systems/EntitySystem.h>

class Game;

// Damages and knockbacks nearby entities.
class ExplosionSystem : public EntitySystem {
public:
    ExplosionSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
