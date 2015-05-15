#pragma once

#include <ostream>

#include "Event.h"

class HealthEvent : public Event {
public:
    HealthEvent(int amount = 100)
            : Event(EVENT_HEALTH, sizeof(HealthEvent)),
              amount(amount) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(HealthEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(HealthEvent));
    }

    inline int getAmount() const {
        return amount;
    }

	friend std::ostream &operator<<(std::ostream &os, const HealthEvent &o) {
        os << "HealthEvent: {" << std::endl;
        os << "    amount: " << o.amount << std::endl;
        os << "}";

        return os;
    }

private:
    int amount;
};
