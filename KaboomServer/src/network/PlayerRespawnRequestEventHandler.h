#pragma once

#include <network/EventHandler.h>

class Game;

class PlayerRespawnRequestEventHandler : public EventHandler
{
public:
	PlayerRespawnRequestEventHandler(Game *game);
	virtual void handle(const Event &e) const;

private:
	Game *game;
};

