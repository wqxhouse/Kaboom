#pragma once

#include "Event.h"

class PlayerInputEvent : public Event {
public:
    PlayerInputEvent(unsigned int playerId,
        bool movingForward,
        bool movingBackward,
        bool movingLeft,
        bool movingRight,
        bool jumping,
        bool firing,
        float yaw,
        float pitch);
    ~PlayerInputEvent();

    virtual EventOpcode getOpcode() const;

    const unsigned int &getPlayerId() const;

    const bool &isMovingForward() const;
    const bool &isMovingBackward() const;
    const bool &isMovingLeft() const;
    const bool &isMovingRight() const;

    const bool &isJumping() const;

    const bool &isFiring() const;

    const float &getYaw() const;
    const float &getPitch() const;

private:
    unsigned int playerId;

    bool movingForward;
    bool movingBackward;
    bool movingLeft;
    bool movingRight;

    bool jumping;

    bool firing;

    float yaw;
    float pitch;
};
