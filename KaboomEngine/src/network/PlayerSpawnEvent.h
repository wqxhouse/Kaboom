#pragma once

#include "Event.h"

class PlayerSpawnEvent : public Event {
public:
    PlayerSpawnEvent();
    PlayerSpawnEvent(unsigned int playerId, float x, float y, float z);
    ~PlayerSpawnEvent();

    const unsigned int &getPlayerId() const;

    const float &getX() const;
    const float &getY() const;
    const float &getZ() const;

    virtual void serialize(char *buf);
    virtual void deserialize(char *buf);

private:
    unsigned int playerId;

    float x;
    float y;
    float z;
};
