#include "PlayerInputEvent.h"

#include <string>

PlayerInputEvent::PlayerInputEvent()
    : PlayerInputEvent(0, false, false, false, false, false, false, 0.0f, 0.0f) {
}

PlayerInputEvent::PlayerInputEvent(unsigned int playerId,
    bool movingForward,
    bool movingBackward,
    bool movingLeft,
    bool movingRight,
    bool jumping,
    bool firing,
    float yaw,
    float pitch)
    : Event(EventOpcode::PLAYER_INPUT, sizeof(PlayerInputEvent)), 
    playerId(playerId),
    movingForward(movingForward),
    movingBackward(movingBackward),
    movingLeft(movingLeft),
    movingRight(movingRight),
    jumping(jumping),
    firing(firing),
    yaw(yaw),
    pitch(pitch) {
}

PlayerInputEvent::~PlayerInputEvent() {
}

const unsigned int &PlayerInputEvent::getPlayerId() const {
    return playerId;
}

const bool &PlayerInputEvent::isMovingForward() const {
    return movingForward;
}

const bool &PlayerInputEvent::isMovingBackward() const {
    return movingBackward;
}

const bool &PlayerInputEvent::isMovingLeft() const {
    return movingLeft;
}

const bool &PlayerInputEvent::isMovingRight() const {
    return movingRight;
}

const bool &PlayerInputEvent::isJumping() const {
    return jumping;
}

const bool &PlayerInputEvent::isFiring() const {
    return firing;
}

const float &PlayerInputEvent::getYaw() const {
    return yaw;
}

const float &PlayerInputEvent::getPitch() const {
    return pitch;
}

void PlayerInputEvent::setPlayerId(unsigned int _playerId){
	playerId = _playerId;
}
void PlayerInputEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(PlayerInputEvent));
}

void PlayerInputEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(PlayerInputEvent));
}

void PlayerInputEvent::updateValues(const PlayerInputEvent &e) {
	playerId = e.getPlayerId();

	movingForward = e.isMovingForward() ? e.isMovingForward() : false;
	movingBackward = e.isMovingBackward() ? e.isMovingBackward() : false;
	movingRight = e.isMovingRight() ? e.isMovingRight() : false;
	movingLeft = e.isMovingLeft() ? e.isMovingLeft() : false;
	jumping = e.isJumping() ? e.isJumping() : false;
	firing = e.isFiring() ? e.isFiring() : false;
	yaw = e.getYaw() ? e.getYaw() : false;
	pitch = e.getPitch() ? e.getPitch() : false;

}
