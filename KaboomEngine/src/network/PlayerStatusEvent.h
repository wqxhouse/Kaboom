#pragma once

#include <ostream>

#include "Event.h"

class PlayerStatusEvent : public Event {
public:
	PlayerStatusEvent(bool isKnockBacked = false, 
					  bool isStaggered = false, 
					  bool isDamaged = false, 
					  bool isAlive = false)
		: Event(EVENT_PLAYER_STATUS, sizeof(PlayerStatusEvent)),
		isKnockBacked(isKnockBacked),
		isStaggered(isStaggered),
		isDamaged(isDamaged),
		isAlive(isAlive){
    }

    inline virtual void serialize(char *buf) const {
		memcpy(buf, this, sizeof(PlayerStatusEvent));
    }

    inline virtual void deserialize(char *buf) {
		memcpy(this, buf, sizeof(PlayerStatusEvent));
    }

    inline bool getIsKnockBacked() const {
        return isKnockBacked;
    }

	inline bool getIsStaggered() const {
		return isStaggered;
	}

	inline bool getIsDamaged() const {
		return isDamaged;
	}

	inline bool getIsAlive() const {
		return isAlive;
	}

	friend std::ostream &operator<<(std::ostream &os, const PlayerStatusEvent &o) {
        os << "PlayerStatusEvent: {" << std::endl;
        os << "    isKnockBacked: " << o.isKnockBacked << std::endl;
		os << "    isStaggered: " << o.isStaggered << std::endl;
		os << "    isDamaged: " << o.isDamaged << std::endl;
		os << "    isAlive: " << o.isAlive << std::endl;
        os << "}";

        return os;
    }

private:
	bool isKnockBacked;
	bool isStaggered;
	bool isDamaged;
	bool isAlive;
	
};
