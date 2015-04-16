#include "RotationEvent.h"

#include <string>

RotationEvent::RotationEvent()
    : Event(EventOpcode::ROTATION, sizeof(RotationEvent)) {
}

RotationEvent::RotationEvent(unsigned int entityId, float yaw, float pitch)
    : Event(EventOpcode::ROTATION, sizeof(RotationEvent)),
    entityId(entityId),
    yaw(yaw),
    pitch(pitch) {
}

RotationEvent::~RotationEvent() {
}

const unsigned int &RotationEvent::getEntityId() const {
    return entityId;
}

const float &RotationEvent::getYaw() const {
    return yaw;
}

const float &RotationEvent::getPitch() const {
    return pitch;
}

void RotationEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(RotationEvent));
}

void RotationEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(RotationEvent));
}
