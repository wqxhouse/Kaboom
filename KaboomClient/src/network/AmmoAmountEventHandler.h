#pragma once

#include <network/EventHandler.h>

class Game;

class AmmoAmountEventHandler : public EventHandler {
public:
	AmmoAmountEventHandler(Game *game);
	virtual void handle(const Event &e) const;

private:
	Game *game;
};
