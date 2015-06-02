#pragma once

#include <network/EventHandler.h>

class Game;

class BindEventHandler : public EventHandler {
public:
    BindEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
