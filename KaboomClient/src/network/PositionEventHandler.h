#pragma once

#include <network/EventHandler.h>

class Game;

class PositionEventHandler : public EventHandler {
public:
    PositionEventHandler(Game *game);
    ~PositionEventHandler();

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
