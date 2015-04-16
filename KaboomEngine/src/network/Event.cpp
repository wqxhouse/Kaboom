#include "Event.h"

#include <string.h>

Event::Event()
    : eventOpcode(EventOpcode::EMPTY),
    byteSize(sizeof(Event)) {
}

Event::~Event() {
}

EventOpcode Event::getOpcode() const {
    return eventOpcode;
};

const unsigned int Event::getByteSize() const {
    return byteSize;
};

void Event::serialize(char *buf) {
    memcpy(buf, this, sizeof(Event));
};

void Event::deserialize(char *buf) {
    memcpy(this, buf, sizeof(Event));
}
