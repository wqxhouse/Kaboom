#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class PositionEvent : public Event {
public:
    PositionEvent(
            unsigned int entityId = 0,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f)
            : Event(EVENT_POSITION, sizeof(PositionEvent)),
              entityId(entityId),
              x(x),
              y(y),
              z(z) {
    }

    inline void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PositionEvent));
    }

    inline void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PositionEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    inline float getX() const {
        return x;
    }

    inline float getY() const {
        return y;
    }

    inline float getZ() const {
        return z;
    }

    friend std::ostream& operator<<(std::ostream &os, const PositionEvent &o) {
        os << "PositionEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;

    float x;
    float y;
    float z;
};
