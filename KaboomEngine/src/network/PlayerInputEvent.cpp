#include "PlayerInputEvent.h"

#include <string>

PlayerInputEvent::PlayerInputEvent(
        unsigned int playerId,
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

unsigned int PlayerInputEvent::getPlayerId() const {
    return playerId;
}

void PlayerInputEvent::setPlayerId(unsigned int playerId) {
    this->playerId = playerId;
}

bool PlayerInputEvent::isMovingForward() const {
    return movingForward;
}

bool PlayerInputEvent::isMovingBackward() const {
    return movingBackward;
}

bool PlayerInputEvent::isMovingLeft() const {
    return movingLeft;
}

bool PlayerInputEvent::isMovingRight() const {
    return movingRight;
}

bool PlayerInputEvent::isJumping() const {
    return jumping;
}

bool PlayerInputEvent::isFiring() const {
    return firing;
}

float PlayerInputEvent::getYaw() const {
    return yaw;
}

float PlayerInputEvent::getPitch() const {
    return pitch;
}

void PlayerInputEvent::serialize(char *buf) const {
    memcpy(buf, this, sizeof(PlayerInputEvent));
}

void PlayerInputEvent::deserialize(char *buf) {
    memcpy(this, buf, sizeof(PlayerInputEvent));
}
