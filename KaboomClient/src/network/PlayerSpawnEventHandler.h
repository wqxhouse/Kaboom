#pragma once

#include <network/EventHandler.h>

#include "../core/Game.h"

class PlayerSpawnEventHandler : public EventHandler {
public:
    PlayerSpawnEventHandler(Game *game);
    ~PlayerSpawnEventHandler();

    virtual void handle(const Event &evt) const;

private:
    Game *game;
};
