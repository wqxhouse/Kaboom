#pragma once

#include <ostream>
#include <string>

#include "Event.h"

class ExplosionEvent : public Event {
public:
    ExplosionEvent(unsigned int bombId = 0)
            : Event(EVENT_EXPLOSION, sizeof(ExplosionEvent)),
              bombId(bombId) {
    }

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(ExplosionEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(ExplosionEvent));
    }

    inline unsigned int getBombId() const {
        return bombId;
    }

    friend std::ostream& operator<<(std::ostream &os, const ExplosionEvent &o) {
        os << "ExplosionEvent: {" << std::endl;
        os << "    bombId: " << o.bombId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int bombId;
};
