#pragma once

#include <network/EventHandler.h>

class Game;

class PlayerSelectionEventHandler : public EventHandler {
public:
    PlayerSelectionEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
