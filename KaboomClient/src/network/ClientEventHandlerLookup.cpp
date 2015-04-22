#include "ClientEventHandlerLookup.h"

#include "PositionEventHandler.h"
#include "SpawnEventHandler.h"

ClientEventHandlerLookup::ClientEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::POSITION, new PositionEventHandler(game));
    addHandler(EventOpcode::ENTITY_SPAWN, new SpawnEventHandler(game));
}

ClientEventHandlerLookup::~ClientEventHandlerLookup() {
}
