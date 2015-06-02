#pragma once

#include <network/EventHandler.h>

class Game;

class AssignEventHandler : public EventHandler {
public:
    AssignEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
