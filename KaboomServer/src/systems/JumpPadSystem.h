#pragma once

#include <systems/EntitySystem.h>

class Game;

class JumpPadSystem : public EntitySystem {
public:
	JumpPadSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
