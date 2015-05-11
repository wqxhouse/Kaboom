#include "PlayerInputEvent.h"

#include <string>

PlayerInputEvent::PlayerInputEvent(
        unsigned int playerId,
        bool movingForward,
        bool movingBackward,
        bool movingLeft,
        bool movingRight,
        bool jumping,
        bool attacking1,
        bool attacking2,
        float yaw,
        float pitch)
        : Event(EVENT_PLAYER_INPUT, sizeof(PlayerInputEvent)),
          playerId(playerId),
          movingForward(movingForward),
          movingBackward(movingBackward),
          movingLeft(movingLeft),
          movingRight(movingRight),
          jumping(jumping),
          attacking1(attacking1),
          attacking2(attacking2),
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

bool PlayerInputEvent::isAttacking1() const {
    return attacking1;
}

bool PlayerInputEvent::isAttacking2() const {
    return attacking2;
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
