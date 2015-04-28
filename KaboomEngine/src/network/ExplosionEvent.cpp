#include "ExplosionEvent.h"

#include <string>

ExplosionEvent::ExplosionEvent()
        : ExplosionEvent(0) {
}

ExplosionEvent::ExplosionEvent(unsigned int bombId)
        : Event(EventOpcode::EXPLOSION, sizeof(ExplosionEvent)),
          bombId(bombId) {
}

ExplosionEvent::~ExplosionEvent() {
}

void ExplosionEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(ExplosionEvent));
}

void ExplosionEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(ExplosionEvent));
}

const unsigned int &ExplosionEvent::getBombId() const {
    return bombId;
}
