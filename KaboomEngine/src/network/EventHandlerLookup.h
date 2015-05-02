#pragma once

#include <unordered_map>

#include "EventHandler.h"
#include "EventOpcode.h"

class EventHandlerLookup {
public:
    void addHandler(const EventOpcode &opcode, EventHandler *handler);

    EventHandler *find(const EventOpcode &opcode) const;

private:
    std::unordered_map<EventOpcode, EventHandler *> handlers;
};
