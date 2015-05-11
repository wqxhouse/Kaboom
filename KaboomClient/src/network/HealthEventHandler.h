#pragma once

#include <network/EventHandler.h>

class Game;

class HealthEventHandler : public EventHandler {
public:
	HealthEventHandler(Game *game);

    virtual void handle(const Event &e) const;

private:
    Game *game;
};
