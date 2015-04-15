#include "PlayerSpawnEventHandler.h"

#include <network/PlayerSpawnEvent.h>

PlayerSpawnEventHandler::PlayerSpawnEventHandler(Game *game)
    : game(game) {
}

PlayerSpawnEventHandler::~PlayerSpawnEventHandler() {
}

void PlayerSpawnEventHandler::handle(const Event &e) const {
    const PlayerSpawnEvent &evt = static_cast<const PlayerSpawnEvent &>(e);

    game->getPlayerFactory().createPlayer(evt.getPlayerId(), evt.getX(), evt.getY(), evt.getZ());
}
