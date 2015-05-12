#pragma once

#include <ostream>

#include "Event.h"

class HealthEvent : public Event {
public:
	HealthEvent(unsigned int healthAmount);
	HealthEvent();

    unsigned int getHealthAmount() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

	friend std::ostream &operator<<(std::ostream &os, const HealthEvent &o) {
        os << "HealthEvent: {" << std::endl;
        os << "    healthAmount: " << o.healthAmount << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int healthAmount;
};
