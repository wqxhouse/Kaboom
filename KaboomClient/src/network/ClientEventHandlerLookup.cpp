#include "ClientEventHandlerLookup.h"

#include "AmmoAmountEventHandler.h"
#include "AssignEventHandler.h"
#include "BindEventHandler.h"
#include "ChatEventHandler.h"
#include "ConnectEventHandler.h"
#include "DestroyEventHandler.h"
#include "DisconnectEventHandler.h"
#include "ExplosionEventHandler.h"
#include "HealthEventHandler.h"
#include "MatchStateEventHandler.h"
#include "PlayerDeathEventHandler.h"
#include "PlayerRenameEventHandler.h"
#include "PlayerRespawnEventHandler.h"
#include "PlayerStatusEventHandler.h"
#include "PositionEventHandler.h"
#include "RotationEventHandler.h"
#include "ScoreEventHandler.h"
#include "SpawnEventHandler.h"
#include "TimeEventHandler.h"

ClientEventHandlerLookup::ClientEventHandlerLookup(Game *game) {
    addHandler(EVENT_CONNECT, new ConnectEventHandler(game));
    addHandler(EVENT_DISCONNECT, new DisconnectEventHandler(game));
    addHandler(EVENT_ASSIGN, new AssignEventHandler(game));
    addHandler(EVENT_BIND, new BindEventHandler(game));
    addHandler(EVENT_MATCH_STATE, new MatchStateEventHandler(game));
    addHandler(EVENT_CHAT, new ChatEventHandler(game));
    addHandler(EVENT_SCORE, new ScoreEventHandler(game));
    addHandler(EVENT_PLAYER_RENAME, new PlayerRenameEventHandler(game));
    addHandler(EVENT_PLAYER_RESPAWN, new PlayerRespawnEventHandler(game));
    addHandler(EVENT_PLAYER_DEATH, new PlayerDeathEventHandler(game));
    addHandler(EVENT_SPAWN, new SpawnEventHandler(game));
    addHandler(EVENT_DESTROY, new DestroyEventHandler(game));
    addHandler(EVENT_POSITION, new PositionEventHandler(game));
    addHandler(EVENT_ROTATION, new RotationEventHandler(game));
    addHandler(EVENT_EXPLOSION, new ExplosionEventHandler(game));
    addHandler(EVENT_HEALTH, new HealthEventHandler(game));
    addHandler(EVENT_AMMO_COUNT, new AmmoAmountEventHandler(game));
    addHandler(EVENT_PLAYER_STATUS, new PlayerStatusEventHandler(game));
    addHandler(EVENT_TIME, new TimeEventHandler(game));
}

ClientEventHandlerLookup::~ClientEventHandlerLookup() {
}
