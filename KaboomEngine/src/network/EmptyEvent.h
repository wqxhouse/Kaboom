#pragma once

#include <string>

#include "Event.h"

class EmptyEvent : public Event {
public:
    EmptyEvent()
            : Event(EVENT_EMPTY, sizeof(EmptyEvent)) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(EmptyEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(EmptyEvent));
    }
};
