#pragma once

#include <network/EventHandler.h>

class Game;

class PlayerInputEventHandler : public EventHandler {
public:
    PlayerInputEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
