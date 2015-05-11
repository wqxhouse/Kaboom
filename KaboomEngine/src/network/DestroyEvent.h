#pragma once

#include <string>

#include "Event.h"

class DestroyEvent : public Event {
public:
    DestroyEvent(unsigned int entityId = 0)
            : Event(EVENT_DESTROY, sizeof(DestroyEvent)),
              entityId(entityId) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(DestroyEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(DestroyEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    friend std::ostream& operator<<(std::ostream &os, const DestroyEvent &o) {
        os << "DestroyEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
};
