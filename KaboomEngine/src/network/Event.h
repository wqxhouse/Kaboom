#pragma once

enum EventOpcode {
    POSITION = 0,
    ROTATION = 1,
    PLAYER_INPUT = 2,
    PLAYER_SPAWN = 3
};

class Event {
public:
    virtual ~Event() {}

    virtual EventOpcode getOpcode() const = 0;
};
