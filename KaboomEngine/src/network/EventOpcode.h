#pragma once

enum EventOpcode {
    EVENT_EMPTY,
    EVENT_CONNECT,
    EVENT_DISCONNECT,
    EVENT_ASSIGN,
    EVENT_BIND,
    EVENT_CHAT,
    EVENT_SCORE,
    EVENT_PLAYER_RENAME,
    EVENT_PLAYER_SELECTION,
    EVENT_PLAYER_INPUT,
    EVENT_PLAYER_RESPAWN,
    EVENT_PLAYER_DEATH,
    EVENT_SPAWN,
    EVENT_DESTROY,
    EVENT_PICKUP_SPAWN,
    EVENT_POSITION,
    EVENT_ROTATION,
    EVENT_EXPLOSION,
    EVENT_HEALTH,
    EVENT_AMMO_COUNT,
    EVENT_EQUIP,
    EVENT_PLAYER_STATUS,
    EVENT_TIME,
    EVENT_MATCH_STATE,
    EVENT_RELOAD_REQUEST,
	EVENT_PLAYER_RESPAWN_REQUEST
};
