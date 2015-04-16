#include "PlayerSpawnEvent.h"

PlayerSpawnEvent::PlayerSpawnEvent(unsigned int playerId, float x, float y, float z)
    : playerId(playerId),
    x(x),
    y(y),
    z(z) {
}

PlayerSpawnEvent::~PlayerSpawnEvent() {
}

EventOpcode PlayerSpawnEvent::getOpcode() const {
    return EventOpcode::PLAYER_SPAWN;
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
