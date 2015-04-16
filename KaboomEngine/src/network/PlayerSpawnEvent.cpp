#include "PlayerSpawnEvent.h"

#include <string>

PlayerSpawnEvent::PlayerSpawnEvent()
    : Event(EventOpcode::PLAYER_SPAWN, sizeof(PlayerSpawnEvent)) {
}

PlayerSpawnEvent::PlayerSpawnEvent(unsigned int playerId, float x, float y, float z)
    : Event(EventOpcode::PLAYER_SPAWN, sizeof(PlayerSpawnEvent)),
    playerId(playerId),
    x(x),
    y(y),
    z(z) {
}

PlayerSpawnEvent::~PlayerSpawnEvent() {
}

const unsigned int &PlayerSpawnEvent::getPlayerId() const {
    return playerId;
}

const float &PlayerSpawnEvent::getX() const {
    return x;
}

const float &PlayerSpawnEvent::getY() const {
    return y;
}

const float &PlayerSpawnEvent::getZ() const {
    return z;
}

void PlayerSpawnEvent::serialize(char * buf) const {
    memcpy(buf, this, sizeof(PlayerSpawnEvent));
}

void PlayerSpawnEvent::deserialize(char * buf) {
    memcpy(this, buf, sizeof(PlayerSpawnEvent));
}
