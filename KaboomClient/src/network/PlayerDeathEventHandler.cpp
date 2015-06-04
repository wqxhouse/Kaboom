#include "PlayerDeathEventHandler.h"

#include <network/PlayerDeathEvent.h>

#include "../core/Game.h"

PlayerDeathEventHandler::PlayerDeathEventHandler(Game *game)
        : game(game) {
}

void PlayerDeathEventHandler::handle(const Event &e) const {
    const PlayerDeathEvent &evt = static_cast<const PlayerDeathEvent &>(e);

    // TODO: Handle player death event
	game->playerAlive = false;
	game->getGameGUIEventHandler()->handle(evt);
}
