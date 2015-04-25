#pragma once

#include <network/EventHandler.h>

class Game;

class SpawnEventHandler : public EventHandler {
public:
    SpawnEventHandler(Game *game);
    ~SpawnEventHandler();

    virtual void handle(const Event &evt) const;

private:
    Game *game;
};
