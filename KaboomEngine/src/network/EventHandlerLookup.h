#pragma once

#include <unordered_map>

#include "EventHandler.h"
#include "EventOpcode.h"

class EventHandlerLookup {
public:
    void addHandler(EventOpcode opcode, EventHandler *handler);

    EventHandler *find(EventOpcode opcode) const;

private:
    std::unordered_map<EventOpcode, EventHandler *> handlers;
};
