#pragma once

#include <string>

#include "Event.h"

class ReloadRequestEvent : public Event {
public:
    ReloadRequestEvent()
            : Event(EVENT_RELOAD_REQUEST, sizeof(ReloadRequestEvent)) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(ReloadRequestEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(ReloadRequestEvent));
    }
};
