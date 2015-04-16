#include "Event.h"

#include <string.h>

Event::Event(EventOpcode opcode, unsigned int byteSize)
    : eventOpcode(opcode),
    byteSize(byteSize) {
}

Event::~Event() {
}

EventOpcode Event::getOpcode() const {
    return eventOpcode;
};

const unsigned int Event::getByteSize() const {
    return byteSize;
};
