#pragma once

#include "EventOpcode.h"

class Event {
public:
    EventOpcode getOpcode() const;

    unsigned int getByteSize() const;

    virtual void serialize(char *buf) const = 0;
    virtual void deserialize(char *buf) = 0;

protected:
    Event(EventOpcode opcode, unsigned int byteSize);

    EventOpcode eventOpcode;
    unsigned int byteSize;
};
