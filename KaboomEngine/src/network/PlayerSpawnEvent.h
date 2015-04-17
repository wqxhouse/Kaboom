#pragma once

#include <ostream>

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

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

    friend std::ostream& operator<<(std::ostream &os, const PlayerSpawnEvent &o) {
        os << "PlayerSpawnEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;

    float x;
    float y;
    float z;
};
