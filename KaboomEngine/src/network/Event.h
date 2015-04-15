#pragma once

enum EventOpcode {
    POSITION = 0,
    ROTATION = 1,
    PLAYER_SPAWN = 2,
};

class Event {
public:
    virtual ~Event() {}

    unsigned char getOpcode() const;
};
