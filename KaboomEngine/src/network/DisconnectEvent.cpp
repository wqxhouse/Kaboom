#include "DisconnectEvent.h"

#include <string>

DisconnectEvent::DisconnectEvent(unsigned int playerId)
        : Event(EVENT_DISCONNECT, sizeof(DisconnectEvent)),
          playerId(playerId) {
}

void DisconnectEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(DisconnectEvent));
}

void DisconnectEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(DisconnectEvent));
}

unsigned int DisconnectEvent::getPlayerId() const {
    return playerId;
}

void DisconnectEvent::setPlayerId(unsigned int playerId) {
    this->playerId = playerId;
}
