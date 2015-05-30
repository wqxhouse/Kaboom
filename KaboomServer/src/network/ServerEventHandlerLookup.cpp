#include "ServerEventHandlerLookup.h"

#include "DisconnectEventHandler.h"
#include "EquipEventHandler.h"
#include "PlayerInputEventHandler.h"
#include "PlayerRenameEventHandler.h"
#include "ReloadRequestEventHandler.h"
#include "PlayerRespawnRequestEventHandler.h"

ServerEventHandlerLookup::ServerEventHandlerLookup(Game *game) {
    addHandler(EVENT_PLAYER_INPUT, new PlayerInputEventHandler(game));
    addHandler(EVENT_PLAYER_RENAME, new PlayerRenameEventHandler(game));
    addHandler(EVENT_DISCONNECT, new DisconnectEventHandler(game));
    addHandler(EVENT_EQUIP, new EquipEventHandler(game));
    addHandler(EVENT_RELOAD_REQUEST, new ReloadRequestEventHandler());
	addHandler(EVENT_PLAYER_RESPAWN_REQUEST, new PlayerReSpawnRequestEventHandler());
}

ServerEventHandlerLookup::~ServerEventHandlerLookup() {
}
