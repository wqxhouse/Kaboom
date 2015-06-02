#pragma once

#include <network/EventHandler.h>

class Game;

class PlayerRespawnEventHandler : public EventHandler {
public:
    PlayerRespawnEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
