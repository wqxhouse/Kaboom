#pragma once

#include "Event.h"

class PlayerSpawnEvent : public Event {
public:
    PlayerSpawnEvent(unsigned int playerId, float x, float y, float z);
    ~PlayerSpawnEvent();

    virtual EventOpcode getOpcode() const;

    const unsigned int &getPlayerId() const;

    const float &getX() const;
    const float &getY() const;
    const float &getZ() const;

private:
    unsigned int playerId;

    float x;
    float y;
    float z;
};
