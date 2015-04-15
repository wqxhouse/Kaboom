#pragma once

#include <network/EventHandler.h>

#include "../core/Game.h"

class PositionEventHandler : public EventHandler {
public:
    PositionEventHandler(Game *game);
    ~PositionEventHandler();

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
