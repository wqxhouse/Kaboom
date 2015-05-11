#pragma once

#include <systems/EntitySystem.h>

class Game;

class PickupSystem : public EntitySystem {
public:
    PickupSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
