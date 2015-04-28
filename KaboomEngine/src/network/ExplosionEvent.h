#pragma once

#include <ostream>

#include "Event.h"

class ExplosionEvent : public Event {
public:
    ExplosionEvent(unsigned int bombId);
    ~ExplosionEvent();
    ExplosionEvent();

    const unsigned int &getBombId() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

    friend std::ostream& operator<<(std::ostream &os, const ExplosionEvent &o) {
        os << "ExplosionEvent: {" << std::endl;
        os << "    bombId: " << o.bombId << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int bombId;
};
