#include "PlayerSpawnEvent.h"

PlayerSpawnEvent::PlayerSpawnEvent() 
	:playerId(0),
	 x(0.0),
	 y(0.0),
	 z(0.0) {
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
