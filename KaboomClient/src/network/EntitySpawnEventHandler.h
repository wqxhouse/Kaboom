#pragma once

#include <network/EventHandler.h>

class Game;

class EntitySpawnEventHandler : public EventHandler {
public:
	EntitySpawnEventHandler(Game *game);
	~EntitySpawnEventHandler();

    virtual void handle(const Event &evt) const;

private:
    Game *game;
};
