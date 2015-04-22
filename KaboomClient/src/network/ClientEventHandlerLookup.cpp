#include "ClientEventHandlerLookup.h"

#include "PositionEventHandler.h"
#include "EntitySpawnEventHandler.h"

ClientEventHandlerLookup::ClientEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::POSITION, new PositionEventHandler(game));
	addHandler(EventOpcode::ENTITY_SPAWN, new EntitySpawnEventHandler(game));
}

ClientEventHandlerLookup::~ClientEventHandlerLookup() {
}
