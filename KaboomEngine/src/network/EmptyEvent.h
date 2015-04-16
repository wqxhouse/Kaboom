#pragma once

#include "Event.h"

class EmptyEvent : public Event {
public:
    EmptyEvent();
    ~EmptyEvent();

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);
};
