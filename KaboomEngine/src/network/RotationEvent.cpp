#include "RotationEvent.h"

#include <string>

RotationEvent::RotationEvent() {
    eventOpcode = EventOpcode::ROTATION;
    byteSize = sizeof(RotationEvent);
}

RotationEvent::RotationEvent(unsigned int entityId, float yaw, float pitch)
    : entityId(entityId),
    yaw(yaw),
    pitch(pitch) {

	eventOpcode = EventOpcode::ROTATION;
	byteSize = sizeof(RotationEvent);
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

void RotationEvent::serialize(char *buf) {
    memcpy(buf, this, sizeof(RotationEvent));
}

void RotationEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(RotationEvent));
}
