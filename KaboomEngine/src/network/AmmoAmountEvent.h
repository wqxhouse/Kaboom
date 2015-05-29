#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class AmmoAmountEvent : public Event {
public:
	AmmoAmountEvent() 
		: Event(EVENT_AMMO_COUNT, sizeof(AmmoAmountEvent)), KaboomV2Amount(0),
		TimeBombAmount(0), StickyBombAmount(0)
	{

	}

	AmmoAmountEvent(int K,int T,int S)
		: Event(EVENT_AMMO_COUNT, sizeof(AmmoAmountEvent)), KaboomV2Amount(K),
		TimeBombAmount(T), StickyBombAmount(S)
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

        os << "    KaboomV2Amount: " << o.KaboomV2Amount << std::endl;
        os << "    TimeBombAmount: " << o.TimeBombAmount << std::endl;
        os << "    StickyBombAmount: " << o.StickyBombAmount << std::endl;
        os << "}";

        return os;
    }

	int getAmmoAmount(int type) const{
		switch (type)
		{
			case 0:
				return KaboomV2Amount;
			case 1:
				return TimeBombAmount;
			case 2:
				return StickyBombAmount;
			default:
				return 0;
		}
	}

private:
	//when we add new bomb, we need to modify this to include it
	int KaboomV2Amount;
	int TimeBombAmount;
	int StickyBombAmount;
};
