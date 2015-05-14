#pragma once

#include <ostream>

#include "Event.h"
#include "../core/EntityType.h"
#include "../math/Quat.h"

class SpawnEvent : public Event {
public:
    SpawnEvent(
            unsigned int entityId = 0,
            EntityType type = NONE,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            Quat rotation = Quat())
            : Event(EVENT_SPAWN, sizeof(SpawnEvent)),
              entityId(entityId),
              type(type),
              x(x),
              y(y),
              z(z),
              rotation(rotation) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(SpawnEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(SpawnEvent));
    }

    inline unsigned int getEntityId() const {
        return entityId;
    }

    inline EntityType getType() const {
        return type;
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

    inline Quat getRotation() const {
        return rotation;
    }

	friend std::ostream& operator<<(std::ostream &os, const SpawnEvent &o) {
        os << "SpawnEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
    EntityType type;

    float x;
    float y;
    float z;

    Quat rotation;
};
