#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class RotationEvent : public Event {
public:
    RotationEvent(
            unsigned int entityId = 0,
            float yaw = 0.0f,
            float pitch = 0.0f)
            : Event(EVENT_ROTATION, sizeof(RotationEvent)),
              entityId(entityId),
              yaw(yaw),
              pitch(pitch) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(RotationEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(RotationEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    inline float getYaw() const {
        return yaw;
    }

    inline float getPitch() const {
        return pitch;
    }

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
