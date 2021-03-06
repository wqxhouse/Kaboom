#pragma once

#include <network/EventHandler.h>

class Game;

class RotationEventHandler : public EventHandler {
public:
    RotationEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
