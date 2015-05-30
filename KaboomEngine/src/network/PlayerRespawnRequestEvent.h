#pragma once

#include <string>

#include "Event.h"

class PlayerRespawnRequestEvent : public Event {
public:
	PlayerRespawnRequestEvent(unsigned int playerId = 0)
		: Event(EVENT_PLAYER_RESPAWN_REQUEST, sizeof(PlayerRespawnRequestEvent)),
		  playerId(playerId) {
	}

	inline virtual void serialize(char *buf) const {
		memcpy(buf, this, sizeof(PlayerRespawnRequestEvent));
	}

	inline virtual void deserialize(char *buf) {
		memcpy(this, buf, sizeof(PlayerRespawnRequestEvent));
	}

	inline unsigned int getPlayerId() const {
		return playerId;
	}

	inline void setPlayerId(unsigned int playerId) {
		this->playerId = playerId;
	}

	friend std::ostream& operator<<(std::ostream &os, const PlayerRespawnRequestEvent &o) {
		os << "PlayerRespawnRequestEvent: {" << std::endl;
		os << "    playerId: " << o.playerId << std::endl;
		os << "}";

		return os;
	}
private:
	unsigned int playerId;
};
