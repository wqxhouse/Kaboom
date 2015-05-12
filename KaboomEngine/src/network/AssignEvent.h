#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class AssignEvent : public Event {
public:
    AssignEvent(unsigned int entityId = 0)
            : Event(EVENT_ASSIGN, sizeof(AssignEvent)),
              entityId(entityId) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(AssignEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(AssignEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

	friend std::ostream &operator<<(std::ostream &os, const AssignEvent &o) {
        os << "AssignEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
};
