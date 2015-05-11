#include "PositionEvent.h"

#include <string>

PositionEvent::PositionEvent(unsigned int entityId, float x, float y, float z)
        : Event(EVENT_POSITION, sizeof(PositionEvent)),
          entityId(entityId),
          x(x),
          y(y),
          z(z) {
}

unsigned int PositionEvent::getEntityId() const {
    return entityId;
}

float PositionEvent::getX() const {
    return x;
}

float PositionEvent::getY() const {
    return y;
}

float PositionEvent::getZ() const {
    return z;
}

void PositionEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(PositionEvent));
}

void PositionEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(PositionEvent));
}
