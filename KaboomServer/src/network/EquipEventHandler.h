#pragma once

#include <network/EventHandler.h>

class Game;

class EquipEventHandler : public EventHandler {
public:
    EquipEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
