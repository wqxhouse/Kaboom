#pragma once

#include <ostream>

#include "Event.h"

class DisconnectEvent : public Event {
public:
    DisconnectEvent(unsigned int playerId = 0)
            : Event(EVENT_DISCONNECT, sizeof(DisconnectEvent)),
              playerId(playerId) {
    }

    inline void serialize(char *buf) const {
        memcpy(buf, this, sizeof(DisconnectEvent));
    }

    inline void deserialize(char *buf) {
        memcpy(this, buf, sizeof(DisconnectEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    inline void setPlayerId(unsigned int playerId) {
        this->playerId = playerId;
    }

    friend std::ostream& operator<<(std::ostream &os, const DisconnectEvent &o) {
        os << "DisconnectEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
};
