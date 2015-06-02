#pragma once

#include <systems/EntitySystem.h>

class Game;

class PickupSpawnSystem : public EntitySystem {
public:
    PickupSpawnSystem(Game *game);

    virtual void process();

private:
    Game *game;
};
