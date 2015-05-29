#pragma once

#include <network/EventHandler.h>

class Game;

class PlayerStatusEventHandler : public EventHandler {
public:
    PlayerStatusEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
