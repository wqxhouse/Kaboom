#include "EventHandlerLookup.h"

void EventHandlerLookup::addHandler(const EventOpcode &opcode, EventHandler *handler) {
    handlers[opcode] = handler;
}

EventHandler *EventHandlerLookup::find(const EventOpcode &opcode) const {
    return handlers.at(opcode);
}
