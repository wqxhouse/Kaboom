#include "SpawnEvent.h"

#include <string>

SpawnEvent::SpawnEvent()
        : SpawnEvent(0, 0.0f, 0.0f, 0.0f, EntityType::NONE, 0) {
}

SpawnEvent::SpawnEvent(unsigned int entityId, float x, float y, float z, EntityType type, int feature)
        : Event(EventOpcode::ENTITY_SPAWN, sizeof(SpawnEvent)),
          entityId(entityId),
          x(x),
          y(y),
          z(z),
          type(type),
          feature(feature) {
}

SpawnEvent::~SpawnEvent() {
}

const unsigned int &SpawnEvent::getEntityId() const {
    return entityId;
}

const float &SpawnEvent::getX() const {
    return x;
}

const float &SpawnEvent::getY() const {
    return y;
}

const float &SpawnEvent::getZ() const {
    return z;
}

const EntityType &SpawnEvent::getType() const {
    return type;
}

const int &SpawnEvent::getFeature() const {
    return feature;
}

void SpawnEvent::serialize(char * buf) const {
    memcpy(buf, this, sizeof(SpawnEvent));
}

void SpawnEvent::deserialize(char * buf) {
    memcpy(this, buf, sizeof(SpawnEvent));
}
