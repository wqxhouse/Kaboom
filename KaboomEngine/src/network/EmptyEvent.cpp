#include "EmptyEvent.h"

#include <string>

EmptyEvent::EmptyEvent()
        : Event(EVENT_EMPTY, sizeof(EmptyEvent)) {
}

void EmptyEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(EmptyEvent));
}

void EmptyEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(EmptyEvent));
}
