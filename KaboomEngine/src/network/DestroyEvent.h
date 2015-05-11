#pragma once

#include <string>

#include "Event.h"

class DestroyEvent : public Event {
public:
    DestroyEvent(unsigned int entityId = 0)
            : Event(EVENT_DESTROY, sizeof(DestroyEvent)),
              entityId(entityId) {
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(DestroyEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(DestroyEvent));
    }

private:
    unsigned int entityId;
};
