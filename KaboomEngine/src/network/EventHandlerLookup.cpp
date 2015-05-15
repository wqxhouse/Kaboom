#include "EventHandlerLookup.h"

void EventHandlerLookup::addHandler(EventOpcode opcode, EventHandler *handler) {
    handlers[opcode] = handler;
}

EventHandler *EventHandlerLookup::find(EventOpcode opcode) const {
    return handlers.at(opcode);
}
