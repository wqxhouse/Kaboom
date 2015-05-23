#pragma once

#include <string>

#include "Event.h"

class PlayerDeathEvent : public Event {
public:
    PlayerDeathEvent()
            : Event(EVENT_PLAYER_DEATH, sizeof(PlayerDeathEvent)) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PlayerDeathEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PlayerDeathEvent));
    }
};
