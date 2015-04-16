#include "PositionEvent.h"

PositionEvent::PositionEvent() :
entityId(0),
x(0.0),
y(0.0),
z(0.0) {
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
