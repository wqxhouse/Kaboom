#pragma once

#include <components/HealthComponent.h>
#include <components/BombContainerComponent.h>
#include <network/HealthEvent.h>
#include <network/AmmoAmountEvent.h>
#include <LibRocketGUIManager.h>

class Game;
class GameGUIEventHandler
{
public:
	GameGUIEventHandler(Game *game);
	void handle(const HealthEvent &e, HealthComponent *healthComponent) const;
	void handle(const AmmoAmountEvent &e, BombContainerComponent *bombConCom) const;
	void changeWeapon(int weapon) const;
	// TODO: add more other events 

private:
	Game *_game;
	LibRocketGUIManager *_guiManager;
};