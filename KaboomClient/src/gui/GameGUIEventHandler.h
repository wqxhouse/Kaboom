#pragma once

#include <components/HealthComponent.h>
#include <components/InventoryComponent.h>
#include <network/HealthEvent.h>
#include <network/AmmoAmountEvent.h>
#include <network/ScoreEvent.h>
#include <network/MatchStateEvent.h>
#include <network/PlayerRenameEvent.h>
#include <network/PlayerDeathEvent.h>
#include <network/PlayerRespawnEvent.h>
#include <network/DisconnectEvent.h>
#include <LibRocketGUIManager.h>
#include <core/Player.h>

class Game;
class GameGUIEventHandler
{
public:
	GameGUIEventHandler(Game *game);
	void handle(const HealthEvent &e, HealthComponent *healthComponent) const;
    void handle(const AmmoAmountEvent &e, InventoryComponent *bombConCom) const;
	void handle(const ScoreEvent &e, std::string name) const;
	void handle(const PlayerRenameEvent &e, Player* player) const;
	void handle(const PlayerDeathEvent &e) const;
	void handle(const PlayerRespawnEvent &e) const;
	int endGame() const;
	void changeWeapon(int weapon) const;
	void showScoreBoard() const;
	void hideScoreBoard() const;
	void changeTime(Game *game) const;
	void changeDeathTime(Game *game)const;
	void updateUserName(std::string * name) const;
	void deletePlayer(const DisconnectEvent &evt)const;
	void deleteAllPlayers()const;
	void preGame(int& color, bool& change)const;
	void inProgress()const;
	int postGame()const;
	void hudScore(const ScoreEvent &evt) const;
	void GameGUIEventHandler::setPreGame() const;
	void resetScoreBoard() const;

	// TODO: add more other events 

private:
	Game *_game;
	LibRocketGUIManager *_guiManager;
	bool goingUP = true;

};