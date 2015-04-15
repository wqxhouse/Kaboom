#pragma once

#include <network/EventHandler.h>
#include <network/PlayerSpawnEvent.h>

#include "../core/Game.h"

class PlayerSpawnEventHandler : public EventHandler<PlayerSpawnEvent> {
public:
    PlayerSpawnEventHandler(Game *game);
    ~PlayerSpawnEventHandler();

    virtual void handle(const PlayerSpawnEvent &evt) const;

private:
    Game *game;
};
