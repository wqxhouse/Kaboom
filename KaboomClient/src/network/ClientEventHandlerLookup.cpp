#include "ClientEventHandlerLookup.h"

#include "DisconnectEventHandler.h"
#include "PositionEventHandler.h"
#include "RotationEventHandler.h"
#include "SpawnEventHandler.h"

ClientEventHandlerLookup::ClientEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::DISCONNECT, new DisconnectEventHandler(game));
    addHandler(EventOpcode::POSITION, new PositionEventHandler(game));
    addHandler(EventOpcode::ROTATION, new RotationEventHandler(game));
    addHandler(EventOpcode::ENTITY_SPAWN, new SpawnEventHandler(game));
}

ClientEventHandlerLookup::~ClientEventHandlerLookup() {
}
