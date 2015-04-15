#include "PlayerSpawnEventHandler.h"

PlayerSpawnEventHandler::PlayerSpawnEventHandler(Game *game)
    : game(game) {
}

PlayerSpawnEventHandler::~PlayerSpawnEventHandler() {
}

void PlayerSpawnEventHandler::handle(const PlayerSpawnEvent &evt) const {
    game->getPlayerFactory().createPlayer(evt.getPlayerId(), evt.getX(), evt.getY(), evt.getZ());
}
