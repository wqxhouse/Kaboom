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
            float pitch = 0.0f);

    unsigned int getPlayerId() const;
    void setPlayerId(unsigned int playerId);

    bool isMovingForward() const;
    bool isMovingBackward() const;
    bool isMovingLeft() const;
    bool isMovingRight() const;

    bool isJumping() const;

    bool isAttacking1() const;
    bool isAttacking2() const;

    float getYaw() const;
    float getPitch() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

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
