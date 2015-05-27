#include "AssignEventHandler.h"

#include <network/AssignEvent.h>

#include "../core/Game.h"

AssignEventHandler::AssignEventHandler(Game *game)
        : game(game) {
}

void AssignEventHandler::handle(const Event &e) const {
    const AssignEvent &evt = static_cast<const AssignEvent &>(e);
	
	std::cout << evt << std::endl;
	game->timeOfAssign=clock();
	game->serverTimeElapsed=evt.getTime();
    game->setCurrentPlayer(game->getPlayers().at(evt.getPlayerId()));
	game->duration = evt.getDuration();
}
