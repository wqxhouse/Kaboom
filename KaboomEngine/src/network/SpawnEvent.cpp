#include "SpawnEvent.h"

#include <string>

SpawnEvent::SpawnEvent(unsigned int entityId, float x, float y, float z, EntityType type, int feature)
        : Event(EVENT_SPAWN, sizeof(SpawnEvent)),
          entityId(entityId),
          x(x),
          y(y),
          z(z),
          type(type),
          feature(feature) {
}

unsigned int SpawnEvent::getEntityId() const {
    return entityId;
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

const EntityType &SpawnEvent::getType() const {
    return type;
}

int SpawnEvent::getFeature() const {
    return feature;
}

void SpawnEvent::serialize(char * buf) const {
    memcpy(buf, this, sizeof(SpawnEvent));
}

void SpawnEvent::deserialize(char * buf) {
    memcpy(this, buf, sizeof(SpawnEvent));
}
