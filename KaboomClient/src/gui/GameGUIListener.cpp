#include "GameGUIListener.h"
#include "../core/Game.h"

GameGUIListener::GameGUIListener(Game *game)
	: _game(game)
{
}

void GameGUIListener::setGameState(GameStateMachine state)
{
	// friend access
	_game->gsm = state;
}