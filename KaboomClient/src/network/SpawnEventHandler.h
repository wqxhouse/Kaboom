#pragma once

#include <network/EventHandler.h>

class Game;

class SpawnEventHandler : public EventHandler {
public:
    SpawnEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
