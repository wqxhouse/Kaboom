#pragma once

#include <network/EventHandler.h>

class Game;

class TimeEventHandler : public EventHandler {
public:
	TimeEventHandler(Game *game);

	virtual void handle(const Event &e) const;

private:
	Game *game;
};
