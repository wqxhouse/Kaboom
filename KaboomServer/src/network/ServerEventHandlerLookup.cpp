#include "ServerEventHandlerLookup.h"

#include "DisconnectEventHandler.h"
#include "EquipEventHandler.h"
#include "PlayerInputEventHandler.h"
#include "ReloadRequestEventHandler.h"

ServerEventHandlerLookup::ServerEventHandlerLookup(Game *game) {
    addHandler(EVENT_PLAYER_INPUT, new PlayerInputEventHandler(game));
    addHandler(EVENT_DISCONNECT, new DisconnectEventHandler(game));
    addHandler(EVENT_EQUIP, new EquipEventHandler(game));
    addHandler(EVENT_RELOAD_REQUEST, new ReloadRequestEventHandler());
}

ServerEventHandlerLookup::~ServerEventHandlerLookup() {
}
