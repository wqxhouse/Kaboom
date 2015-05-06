#pragma once

enum EventOpcode {
    EVENT_EMPTY = 0,
    EVENT_POSITION = 1,
    EVENT_ROTATION = 2,
    EVENT_PLAYER_INPUT = 3,
    EVENT_SPAWN = 4,
    EVENT_DISCONNECT = 5,
    EVENT_ASSIGN = 6,
    EVENT_EXPLOSION = 7,
};
