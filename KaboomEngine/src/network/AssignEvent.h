#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class AssignEvent : public Event {
public:
    AssignEvent(unsigned int playerId = 0)
            : Event(EVENT_ASSIGN, sizeof(AssignEvent)),
              playerId(playerId) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(AssignEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(AssignEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    friend std::ostream &operator<<(std::ostream &os, const AssignEvent &o) {
        os << "AssignEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
};
