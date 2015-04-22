#pragma once

#include <ostream>

#include "Event.h"

class DeleteEvent : public Event {
public:
	
	DeleteEvent(unsigned int entityId);
	~DeleteEvent();
	DeleteEvent();

    const unsigned int &getEntityId() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

	friend std::ostream& operator<<(std::ostream &os, const DeleteEvent &o) {
        os << "RotationEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
};
