#pragma once

#include <network/EventHandler.h>

class Game;

class DisconnectEventHandler : public EventHandler {
public:
    DisconnectEventHandler(Game *game);
    ~DisconnectEventHandler();

    virtual void handle(const Event &evt) const;

private:
    Game *game;
};
