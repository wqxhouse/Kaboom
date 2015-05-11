#pragma once

#include <network/EventHandler.h>

class Game;

class DestroyEventHandler : public EventHandler {
public:
    DestroyEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
