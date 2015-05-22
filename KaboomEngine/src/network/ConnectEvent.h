#pragma once

#include <ostream>

#include "Event.h"

class ConnectEvent : public Event {
public:
    ConnectEvent(unsigned int playerId = 0)
            : Event(EVENT_CONNECT, sizeof(ConnectEvent)),
              playerId(playerId) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(ConnectEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(ConnectEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    friend std::ostream& operator<<(std::ostream &os, const ConnectEvent &o) {
        os << "ConnectEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
};
