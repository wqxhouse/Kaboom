#pragma once

#include <network/EventHandler.h>

class Game;

class ExplosionEventHandler : public EventHandler {
public:
    ExplosionEventHandler(Game *game);
    ~ExplosionEventHandler();

    virtual void handle(const Event &evt) const;

private:
    Game *game;
};
