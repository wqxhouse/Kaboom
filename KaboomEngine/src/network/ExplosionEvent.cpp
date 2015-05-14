#include "ExplosionEvent.h"

#include <string>

ExplosionEvent::ExplosionEvent(unsigned int bombId)
        : Event(EVENT_EXPLOSION, sizeof(ExplosionEvent)),
          bombId(bombId) {
}

void ExplosionEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(ExplosionEvent));
}

void ExplosionEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(ExplosionEvent));
}

unsigned int ExplosionEvent::getBombId() const {
    return bombId;
}
