#pragma once

#include <ostream>

#include "Event.h"
#include "../core/EntityType.h"

class SpawnEvent : public Event {
public:
    SpawnEvent(
            unsigned int entityId = 0,
            EntityType type = NONE,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            float yaw = 0.0f,
            float pitch = 0.0f);

    unsigned int getEntityId() const;
    const EntityType &getType() const;

    float getX() const;
    float getY() const;
    float getZ() const;

    float getYaw() const;
    float getPitch() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

	friend std::ostream& operator<<(std::ostream &os, const SpawnEvent &o) {
        os << "EntitySpawnEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    type: " << o.type << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
        os << "    yaw: " << o.yaw << std::endl;
        os << "    pitch: " << o.pitch << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;
    EntityType type;

    float x;
    float y;
    float z;

    float yaw;
    float pitch;
};
