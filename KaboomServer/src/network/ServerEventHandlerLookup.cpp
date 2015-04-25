#include "ServerEventHandlerLookup.h"

#include "DisconnectEventHandler.h"
#include "PlayerInputEventHandler.h"

ServerEventHandlerLookup::ServerEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::PLAYER_INPUT, new PlayerInputEventHandler(game));
    addHandler(EventOpcode::DISCONNECT, new DisconnectEventHandler(game));
}

ServerEventHandlerLookup::~ServerEventHandlerLookup() {
}
