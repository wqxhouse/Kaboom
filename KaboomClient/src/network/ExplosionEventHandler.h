#pragma once

#include <network/EventHandler.h>

class Game;

class ExplosionEventHandler : public EventHandler {
public:
    ExplosionEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
