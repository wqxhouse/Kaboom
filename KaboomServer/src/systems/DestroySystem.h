#pragma once

#include <systems/EntitySystem.h>

#include "../core/Game.h"

class DestroySystem : public EntitySystem {
public:
    DestroySystem(Game* game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
