#pragma once

#include <unordered_map>

#include "Event.h"
#include "EventHandler.h"

class EventHandlerLookup {
public:
    EventHandlerLookup();
    ~EventHandlerLookup();

    void addHandler(const EventOpcode &opcode, EventHandler *handler);

    EventHandler *find(const EventOpcode &opcode) const;

private:
    std::unordered_map<EventOpcode, EventHandler *> handlers;
};
