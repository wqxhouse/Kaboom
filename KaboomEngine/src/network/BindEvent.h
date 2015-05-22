#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class BindEvent : public Event {
public:
    BindEvent(unsigned int playerId = 0, unsigned int entityId = 0)
            : Event(EVENT_BIND, sizeof(BindEvent)),
              playerId(playerId),
              entityId(entityId) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(BindEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(BindEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    friend std::ostream &operator<<(std::ostream &os, const BindEvent &o) {
        os << "BindEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;
    unsigned int entityId;
};
