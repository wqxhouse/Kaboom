#pragma once

#include <systems/EntitySystem.h>

class Game;

class InitializationSystem : public EntitySystem {
public:
    InitializationSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
