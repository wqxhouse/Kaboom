#pragma once

#include "Event.h"

class EventHandler {
public:
    virtual ~EventHandler() {}

    virtual void handle(const Event &e) const = 0;
};
