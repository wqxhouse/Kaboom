#include "PlayerRespawnEventHandler.h"

#include <network/PlayerRespawnEvent.h>

#include "../core/Game.h"

PlayerRespawnEventHandler::PlayerRespawnEventHandler(Game *game)
        : game(game) {
}

void PlayerRespawnEventHandler::handle(const Event &e) const {
    const PlayerRespawnEvent &evt = static_cast<const PlayerRespawnEvent &>(e);

    // TODO: Handle player respawn event
	game->getGameGUIEventHandler()->handle(evt);
}
