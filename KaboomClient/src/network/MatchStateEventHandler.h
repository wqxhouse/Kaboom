#pragma once

#include <network/EventHandler.h>

class Game;

class MatchStateEventHandler : public EventHandler {
public:
    MatchStateEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
