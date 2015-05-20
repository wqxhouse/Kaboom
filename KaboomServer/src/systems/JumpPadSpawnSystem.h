#pragma once

#include <systems/EntitySystem.h>

class Game;

class JumpPadSpawnSystem : public EntitySystem {
public:
	JumpPadSpawnSystem(Game *game);

    virtual void process();

private:
    Game *game;
};
