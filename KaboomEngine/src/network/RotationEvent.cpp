#include "RotationEvent.h"

RotationEvent::RotationEvent(unsigned int entityId, float yaw, float pitch)
    : entityId(entityId),
    yaw(yaw),
    pitch(pitch) {
}

RotationEvent::~RotationEvent() {
}

unsigned char RotationEvent::getOpcode() const {
    return EventOpcode::ROTATION;
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
