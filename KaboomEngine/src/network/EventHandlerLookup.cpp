#include "EventHandlerLookup.h"

EventHandlerLookup::EventHandlerLookup() {
}

EventHandlerLookup::~EventHandlerLookup() {
}

void EventHandlerLookup::addHandler(const EventOpcode &opcode, EventHandler *handler) {
    handlers[opcode];
}

EventHandler *EventHandlerLookup::find(const EventOpcode &opcode) const {
    return handlers.at(opcode);
}
