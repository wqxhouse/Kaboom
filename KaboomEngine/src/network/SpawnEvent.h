#pragma once

#include <ostream>

#include "Event.h"
#include "../core/EntityType.h"
#include "../math/Quat.h"
#include "../math/Vec3.h"

class SpawnEvent : public Event {
public:
    SpawnEvent(
            unsigned int entityId = 0,
            EntityType type = NONE,
            const Vec3 &position = Vec3(),
            Quat rotation = Quat())
            : Event(EVENT_SPAWN, sizeof(SpawnEvent)),
              entityId(entityId),
              type(type),
              position(position),
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

    inline const Vec3 &getPosition() const {
        return position;
    }

    inline Quat getRotation() const {
        return rotation;
    }

	friend std::ostream& operator<<(std::ostream &os, const SpawnEvent &o) {
        os << "SpawnEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "    x: " << o.position.x << std::endl;
        os << "    y: " << o.position.y << std::endl;
        os << "    z: " << o.position.z << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
    EntityType type;

    Vec3 position;
    Quat rotation;
};
