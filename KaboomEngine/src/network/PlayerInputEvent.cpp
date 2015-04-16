#include "PlayerInputEvent.h"

PlayerInputEvent::PlayerInputEvent() :
	playerId(0),
	movingForward(false),
	movingBackward(false),
	movingLeft(false),
	movingRight(false),
	jumping(false),
	firing(false),
	yaw(false),
	pitch(false) {
}

PlayerInputEvent::PlayerInputEvent(unsigned int playerId,
    bool movingForward,
    bool movingBackward,
    bool movingLeft,
    bool movingRight,
    bool jumping,
    bool firing,
    float yaw,
    float pitch) :
	playerId(playerId),
    movingForward(movingForward),
    movingBackward(movingBackward),
    movingLeft(movingLeft),
    movingRight(movingRight),
    jumping(jumping),
    firing(firing),
    yaw(yaw),
    pitch(pitch) {

	eventOpcode = EventOpcode::PLAYER_INPUT;
	byteSize = sizeof(PlayerInputEvent);
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
