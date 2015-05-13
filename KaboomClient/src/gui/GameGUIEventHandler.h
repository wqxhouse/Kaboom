#pragma once

#include <components/HealthComponent.h>
#include <network/HealthEvent.h>
#include <LibRocketGUIManager.h>

class Game;
class GameGUIEventHandler
{
public:
	GameGUIEventHandler(Game *game);
	void handle(const HealthEvent &e, HealthComponent *healthComponent) const;

	// TODO: add more other events 

private:
	Game *_game;
	LibRocketGUIManager *_guiManager;
};