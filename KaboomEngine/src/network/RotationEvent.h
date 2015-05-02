#pragma once

#include <ostream>

#include "Event.h"

class RotationEvent : public Event {
public:
    RotationEvent(
            unsigned int entityId = 0,
            float yaw = 0.0f,
            float pitch = 0.0f);

    unsigned int getEntityId() const;

    float getYaw() const;
    float getPitch() const;

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
