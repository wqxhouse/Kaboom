#pragma once

#include <string>

#include "Event.h"

class PlayerRespawnEvent : public Event {
public:
    PlayerRespawnEvent()
            : Event(EVENT_PLAYER_RESPAWN, sizeof(PlayerRespawnEvent)) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PlayerRespawnEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PlayerRespawnEvent));
    }
};
