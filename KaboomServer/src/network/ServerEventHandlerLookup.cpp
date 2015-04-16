#include "ServerEventHandlerLookup.h"

#include "PlayerInputEventHandler.h"

ServerEventHandlerLookup::ServerEventHandlerLookup(Game *game) {
    addHandler(EventOpcode::PLAYER_INPUT, new PlayerInputEventHandler(game));
}

ServerEventHandlerLookup::~ServerEventHandlerLookup() {
}
