#pragma once

#include <network/EventHandler.h>

class Game;

class ScoreEventHandler : public EventHandler {
public:
    ScoreEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
