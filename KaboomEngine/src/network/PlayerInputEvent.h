#pragma once

#include <ostream>

#include "Event.h"

class PlayerInputEvent : public Event {
public:
    PlayerInputEvent(
            unsigned int playerId = 0,
            bool movingForward = false,
            bool movingBackward = false,
            bool movingLeft = false,
            bool movingRight = false,
            bool jumping = false,
            bool attacking1 = false,
            bool attacking2 = false,
            float yaw = 0.0f,
            float pitch = 0.0f)
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

    inline virtual void serialize(char *buf) const {
        memcpy(buf, this, sizeof(PlayerInputEvent));
    }

    inline virtual void deserialize(char *buf) {
        memcpy(this, buf, sizeof(PlayerInputEvent));
    }

    inline unsigned int getPlayerId() const {
        return playerId;
    }

    inline void setPlayerId(unsigned int playerId) {
        this->playerId = playerId;
    }

    inline bool isMovingForward() const {
        return movingForward;
    }

    inline bool isMovingBackward() const {
        return movingBackward;
    }

    inline bool isMovingLeft() const {
        return movingLeft;
    }

    inline bool isMovingRight() const {
        return movingRight;
    }

    inline bool isJumping() const {
        return jumping;
    }

    inline bool isAttacking1() const {
        return attacking1;
    }

    inline bool isAttacking2() const {
        return attacking2;
    }

    inline float getYaw() const {
        return yaw;
    }

    inline float getPitch() const {
        return pitch;
    }

    friend std::ostream& operator<<(std::ostream &os, const PlayerInputEvent &o) {
        os << "PlayerInputEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    movingForward: " << o.movingForward << std::endl;
        os << "    movingBackward: " << o.movingBackward << std::endl;
        os << "    movingLeft: " << o.movingLeft << std::endl;
        os << "    movingRight: " << o.movingRight << std::endl;
        os << "    jumping: " << o.jumping << std::endl;
        os << "    attacking1: " << o.attacking1 << std::endl;
        os << "    attacking2: " << o.attacking2 << std::endl;
        os << "    yaw: " << o.yaw << std::endl;
        os << "    pitch: " << o.pitch << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int playerId;

    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;

    bool jumping;

    bool attacking1;
    bool attacking2;

    float yaw;
    float pitch;
};
