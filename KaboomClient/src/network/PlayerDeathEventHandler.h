#pragma once

#include <network/EventHandler.h>

class Game;

class PlayerDeathEventHandler : public EventHandler {
public:
    PlayerDeathEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
