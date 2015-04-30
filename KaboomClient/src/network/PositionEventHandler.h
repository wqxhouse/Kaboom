#pragma once

#include <network/EventHandler.h>

class Game;

class PositionEventHandler : public EventHandler {
public:
    PositionEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
