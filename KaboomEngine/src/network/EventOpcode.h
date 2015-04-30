#pragma once

enum EventOpcode {
    EMPTY = 0,
    POSITION = 1,
    ROTATION = 2,
    PLAYER_INPUT = 3,
    ENTITY_SPAWN = 4,
    DISCONNECT = 5,
	ASSIGN_ENTITY = 6,
    EXPLOSION = 7,
};
