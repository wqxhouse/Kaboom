#include "SpawnEvent.h"

#include <string>

SpawnEvent::SpawnEvent(
        unsigned int entityId,
        EntityType type,
        float x,
        float y,
        float z,
        float yaw,
        float pitch)
        : Event(EVENT_SPAWN, sizeof(SpawnEvent)),
          entityId(entityId),
          type(type),
          x(x),
          y(y),
          z(z),
          yaw(yaw),
          pitch(pitch) {
}

unsigned int SpawnEvent::getEntityId() const {
    return entityId;
}

const EntityType &SpawnEvent::getType() const {
    return type;
}

float SpawnEvent::getX() const {
    return x;
}

float SpawnEvent::getY() const {
    return y;
}

float SpawnEvent::getZ() const {
    return z;
}

float SpawnEvent::getYaw() const {
    return yaw;
}

float SpawnEvent::getPitch() const {
    return pitch;
}

void SpawnEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(SpawnEvent));
}

void SpawnEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(SpawnEvent));
}
