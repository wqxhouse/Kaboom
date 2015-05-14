#pragma once

#include <ostream>
#include <string>

#include "Event.h"
#include "../math/Quat.h"

class RotationEvent : public Event {
public:
    RotationEvent(unsigned int entityId = 0, Quat rotation = Quat())
            : Event(EVENT_ROTATION, sizeof(RotationEvent)),
              entityId(entityId),
              rotation(rotation) {
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

    inline Quat getRotation() const {
        return rotation;
    }

    friend std::ostream& operator<<(std::ostream &os, const RotationEvent &o) {
        os << "RotationEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    x: " << o.rotation.x << std::endl;
        os << "    y: " << o.rotation.y << std::endl;
        os << "    z: " << o.rotation.z << std::endl;
        os << "    w: " << o.rotation.w << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;

    Quat rotation;
};
