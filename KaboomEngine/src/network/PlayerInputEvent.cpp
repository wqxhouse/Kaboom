#include "PlayerInputEvent.h"

PlayerInputEvent::PlayerInputEvent(unsigned int playerId,
    bool movingForward,
    bool movingBackward,
    bool movingLeft,
    bool movingRight,
    bool jumping,
    bool firing,
    float yaw,
    float pitch) :
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

EventOpcode PlayerInputEvent::getOpcode() const {
    return EventOpcode::PLAYER_INPUT;
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
