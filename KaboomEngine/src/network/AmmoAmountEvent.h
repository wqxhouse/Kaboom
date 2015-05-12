#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class AmmoAmountEvent : public Event {
public:
	AmmoAmountEvent(int K,int T,int S)
		: Event(EVENT_POSITION, sizeof(AmmoAmountEvent)), KaboomV2Amount(K),
		TimeBomb(T), StickyBomb(S)
             {
    }

    inline virtual void serialize(char *buf) const {
		memcpy(buf, this, sizeof(AmmoAmountEvent));
    }

    inline virtual void deserialize(char *buf) {
		memcpy(this, buf, sizeof(AmmoAmountEvent));
    }


	friend std::ostream& operator<<(std::ostream &os, const AmmoAmountEvent &o) {
        os << "AmmoAmountEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
        os << "}";

        return os;
    }

private:


	int KaboomV2Amount;
	int TimeBomb;
	int StickyBomb;
};
