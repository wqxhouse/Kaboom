#include "PositionEvent.h"

#include <string>

PositionEvent::PositionEvent()
    : Event(EventOpcode::POSITION, sizeof(PositionEvent)) {
}

PositionEvent::PositionEvent(unsigned int entityId, float x, float y, float z)
    : Event(EventOpcode::POSITION, sizeof(PositionEvent)),
    entityId(entityId),
    x(x),
    y(y),
    z(z) {
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

void PositionEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(PositionEvent));
}

void PositionEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(PositionEvent));
}
