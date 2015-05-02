#include "RotationEvent.h"

#include <string>

RotationEvent::RotationEvent(unsigned int entityId, float yaw, float pitch)
        : Event(EventOpcode::ROTATION, sizeof(RotationEvent)),
          entityId(entityId),
          yaw(yaw),
          pitch(pitch) {
}

unsigned int RotationEvent::getEntityId() const {
    return entityId;
}

float RotationEvent::getYaw() const {
    return yaw;
}

float RotationEvent::getPitch() const {
    return pitch;
}

void RotationEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(RotationEvent));
}

void RotationEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(RotationEvent));
}
