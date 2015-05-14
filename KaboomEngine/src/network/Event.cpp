#include "Event.h"

#include <string.h>

Event::Event(EventOpcode opcode, unsigned int byteSize)
        : eventOpcode(opcode),
          byteSize(byteSize) {
}

EventOpcode Event::getOpcode() const {
    return eventOpcode;
};

unsigned int Event::getByteSize() const {
    return byteSize;
};
