#pragma once

#include <network/EventHandler.h>

class Game;

class ConnectEventHandler : public EventHandler {
public:
    ConnectEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
