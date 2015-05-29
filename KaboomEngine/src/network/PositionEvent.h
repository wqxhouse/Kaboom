#pragma once

#include <ostream>
#include <string>

#include "Event.h"
#include "../math/Vec3.h"

class PositionEvent : public Event {
public:
    PositionEvent(unsigned int entityId = 0, const Vec3 &position = Vec3())
            : Event(EVENT_POSITION, sizeof(PositionEvent)),
              entityId(entityId),
              position(position) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PositionEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PositionEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    inline const Vec3 &getPosition() const {
        return position;
    }

    friend std::ostream& operator<<(std::ostream &os, const PositionEvent &o) {
        os << "PositionEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    x: " << o.position.x << std::endl;
        os << "    y: " << o.position.y << std::endl;
        os << "    z: " << o.position.z << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
    Vec3 position;
};
