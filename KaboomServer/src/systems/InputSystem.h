#pragma once

#include <systems/EntitySystem.h>

class Game;

class InputSystem : public EntitySystem {
public:
    InputSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
