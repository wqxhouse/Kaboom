#include "PositionEvent.h"

#include <string>

PositionEvent::PositionEvent() {
    eventOpcode = EventOpcode::POSITION;
    byteSize = sizeof(PositionEvent);
}

PositionEvent::PositionEvent(unsigned int entityId, float x, float y, float z)
    : entityId(entityId),
    x(x),
    y(y),
    z(z) {

	eventOpcode = EventOpcode::POSITION;
	byteSize = sizeof(PositionEvent);
}

PositionEvent::~PositionEvent() {
}

const unsigned int &PositionEvent::getEntityId() const {
    return entityId;
}

const float &PositionEvent::getX() const {
    return x;
}

const float &PositionEvent::getY() const {
    return y;
}

const float &PositionEvent::getZ() const {
    return z;
}

void PositionEvent::serialize(char *buf) {
    memcpy(buf, this, sizeof(PositionEvent));
}

void PositionEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(PositionEvent));
}
