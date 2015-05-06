#pragma once

#include <ostream>

#include "Event.h"

class AssignEvent : public Event {
public:
	AssignEvent(unsigned int entityId = 0);

    unsigned int getEntityId() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

	friend std::ostream &operator<<(std::ostream &os, const AssignEvent &o) {
        os << "AssignEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
};
