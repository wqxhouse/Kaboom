#pragma once

#include "EventOpcode.h"

class Event {
public:
    inline EventOpcode getOpcode() const {
        return eventOpcode;
    };

    inline unsigned int getByteSize() const {
        return byteSize;
    };

    virtual void serialize(char *buf) const = 0;
    virtual void deserialize(char *buf) = 0;

protected:
    Event(EventOpcode opcode, unsigned int byteSize)
            : eventOpcode(opcode),
              byteSize(byteSize) {
    }

    EventOpcode eventOpcode;
    unsigned int byteSize;
};
