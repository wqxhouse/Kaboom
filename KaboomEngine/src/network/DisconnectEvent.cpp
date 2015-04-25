#include "DisconnectEvent.h"

#include <string>

DisconnectEvent::DisconnectEvent()
        : DisconnectEvent(0) {
}

DisconnectEvent::DisconnectEvent(unsigned int id)
        : Event(EventOpcode::DISCONNECT, sizeof(DisconnectEvent)),
          playerId(id) {
}

DisconnectEvent::~DisconnectEvent() {
}

void DisconnectEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(DisconnectEvent));
}

void DisconnectEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(DisconnectEvent));
}

const unsigned int &DisconnectEvent::getPlayerId() const {
    return playerId;
}

void DisconnectEvent::setPlayerId(const unsigned int &playerId) {
    this->playerId = playerId;
}
