#pragma once

#include <network/EventHandler.h>

#include "../core/Game.h"

class PlayerInputEventHandler : public EventHandler {
public:
    PlayerInputEventHandler(Game *game);
    ~PlayerInputEventHandler();

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
