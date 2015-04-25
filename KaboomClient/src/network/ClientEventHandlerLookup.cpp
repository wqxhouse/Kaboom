#include "ClientEventHandlerLookup.h"

#include "DeleteEventHandler.h"
#include "PositionEventHandler.h"
#include "RotationEventHandler.h"
#include "SpawnEventHandler.h"

ClientEventHandlerLookup::ClientEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::POSITION, new PositionEventHandler(game));
    addHandler(EventOpcode::ROTATION, new RotationEventHandler(game));
    addHandler(EventOpcode::ENTITY_SPAWN, new SpawnEventHandler(game));
	addHandler(EventOpcode::DELETE_ENTITY, new DeleteEventHandler(game));
}

ClientEventHandlerLookup::~ClientEventHandlerLookup() {
}
