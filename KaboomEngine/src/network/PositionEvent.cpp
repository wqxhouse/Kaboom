#include "PositionEvent.h"

PositionEvent::PositionEvent(unsigned int entityId, float x, float y, float z)
    : entityId(entityId),
    x(x),
    y(y),
    z(z) {
}

PositionEvent::~PositionEvent() {
}

unsigned char PositionEvent::getOpcode() const {
    return EventOpcode::POSITION;
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
