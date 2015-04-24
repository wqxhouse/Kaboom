#include "ClientEventHandlerLookup.h"

#include "PositionEventHandler.h"
#include "SpawnEventHandler.h"
#include "DeleteEventHandler.h"

ClientEventHandlerLookup::ClientEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::POSITION, new PositionEventHandler(game));
    addHandler(EventOpcode::ENTITY_SPAWN, new SpawnEventHandler(game));
	addHandler(EventOpcode::DELETE_ENTITY, new DeleteEventHandler(game));
}

ClientEventHandlerLookup::~ClientEventHandlerLookup() {
}
