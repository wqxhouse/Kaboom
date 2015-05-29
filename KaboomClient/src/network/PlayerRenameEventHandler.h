#pragma once

#include <network/EventHandler.h>

class Game;

class PlayerRenameEventHandler : public EventHandler {
public:
    PlayerRenameEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
