#include "RotationEvent.h"

RotationEvent::RotationEvent() :
entityId(0),
yaw(0.0),
pitch(0.0) {
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
