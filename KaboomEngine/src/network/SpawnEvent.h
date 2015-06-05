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
            bool pickup = false,
            const Vec3 &position = Vec3(),
            Quat rotation = Quat(),
            unsigned int ownerId = 0)
            : Event(EVENT_SPAWN, sizeof(SpawnEvent)),
              entityId(entityId),
              type(type),
              pickup(pickup),
              position(position),
              rotation(rotation),
              ownerId(ownerId) {
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

    inline bool isPickup() const {
        return pickup;
    }

    inline const Vec3 &getPosition() const {
        return position;
    }

    inline Quat getRotation() const {
        return rotation;
    }

    inline unsigned int getOwnerId() const {
        return ownerId;
    }

	friend std::ostream& operator<<(std::ostream &os, const SpawnEvent &o) {
        os << "SpawnEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "    pickup: " << o.pickup << std::endl;
        os << "    position: {" << std::endl;
        os << "        x: " << o.position.x << std::endl;
        os << "        y: " << o.position.x << std::endl;
        os << "        z: " << o.position.x << std::endl;
        os << "    }" << std::endl;
        os << "    rotation: {" << std::endl;
        os << "        x: " << o.rotation.x << std::endl;
        os << "        y: " << o.rotation.y << std::endl;
        os << "        z: " << o.rotation.z << std::endl;
        os << "        w: " << o.rotation.w << std::endl;
        os << "    }" << std::endl;
        os << "    ownerId: " << o.ownerId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
    EntityType type;
    bool pickup;
    Vec3 position;
    Quat rotation;
    unsigned int ownerId;
};
