#pragma once

#include <network/EventHandler.h>

#include "../core/Game.h"

class EntitySpawnEventHandler : public EventHandler {
public:
	EntitySpawnEventHandler(Game *game);
	~EntitySpawnEventHandler();

    virtual void handle(const Event &evt) const;

private:
    Game *game;
};
