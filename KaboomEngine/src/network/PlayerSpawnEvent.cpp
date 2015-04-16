#include "PlayerSpawnEvent.h"

#include <string>

PlayerSpawnEvent::PlayerSpawnEvent() {
    eventOpcode = EventOpcode::PLAYER_SPAWN;
    byteSize = sizeof(PlayerSpawnEvent);
}

PlayerSpawnEvent::PlayerSpawnEvent(unsigned int playerId, float x, float y, float z)
    : playerId(playerId),
    x(x),
    y(y),
    z(z) {

	eventOpcode = EventOpcode::PLAYER_SPAWN;
	byteSize = sizeof(PlayerSpawnEvent);
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

void PlayerSpawnEvent::serialize(char * buf) {
    memcpy(buf, this, sizeof(PlayerSpawnEvent));
}

void PlayerSpawnEvent::deserialize(char * buf) {
    memcpy(this, buf, sizeof(PlayerSpawnEvent));
}
