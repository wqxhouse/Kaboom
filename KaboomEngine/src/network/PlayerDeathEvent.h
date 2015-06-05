#pragma once

#include <string>

#include "Event.h"

class PlayerDeathEvent : public Event {
public:
    PlayerDeathEvent(unsigned int playerId = 0)
            : Event(EVENT_PLAYER_DEATH, sizeof(PlayerDeathEvent)),
			  playerId(playerId) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PlayerDeathEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PlayerDeathEvent));
    }

	inline unsigned int getPlayerId() const {
		return playerId;
	}

private:
	unsigned int playerId;
};
