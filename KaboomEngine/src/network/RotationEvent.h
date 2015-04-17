#pragma once

#include <ostream>

#include "Event.h"

class RotationEvent : public Event {
public:
	RotationEvent();
    RotationEvent(unsigned int entityId, float yaw, float pitch);
    ~RotationEvent();

    const unsigned int &getEntityId() const;

    const float &getYaw() const;
    const float &getPitch() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

    friend std::ostream& operator<<(std::ostream &os, const RotationEvent &o) {
        os << "RotationEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    yaw: " << o.yaw << std::endl;
        os << "    pitch: " << o.pitch << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;

    float yaw;
    float pitch;
};
