#include "ClientEventHandlerLookup.h"

#include "PositionEventHandler.h"
#include "PlayerSpawnEventHandler.h"

ClientEventHandlerLookup::ClientEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::POSITION, new PositionEventHandler(game));
    addHandler(EventOpcode::PLAYER_SPAWN, new PlayerSpawnEventHandler(game));
}

ClientEventHandlerLookup::~ClientEventHandlerLookup() {
}
