#pragma once

#include <components/HealthComponent.h>
#include <components/InventoryComponent.h>
#include <network/HealthEvent.h>
#include <network/AmmoAmountEvent.h>
#include <network/ScoreEvent.h>
#include <LibRocketGUIManager.h>

class Game;
class GameGUIEventHandler
{
public:
	GameGUIEventHandler(Game *game);
	void handle(const HealthEvent &e, HealthComponent *healthComponent) const;
    void handle(const AmmoAmountEvent &e, InventoryComponent *bombConCom) const;
	void handle(const ScoreEvent &e) const;
	void changeWeapon(int weapon) const;
	void showScoreBoard() const;
	void hideScoreBoard() const;
	// TODO: add more other events 

private:
	Game *_game;
	LibRocketGUIManager *_guiManager;
};