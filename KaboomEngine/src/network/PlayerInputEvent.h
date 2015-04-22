#pragma once

#include <ostream>

#include "Event.h"

class PlayerInputEvent : public Event {
public:
    PlayerInputEvent();
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

    const unsigned int &getPlayerId() const;

    const bool &isMovingForward() const;
    const bool &isMovingBackward() const;
    const bool &isMovingLeft() const;
    const bool &isMovingRight() const;

    const bool &isJumping() const;

    const bool &isFiring() const;

    const float &getYaw() const;
    const float &getPitch() const;

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

	virtual void setPlayerId(unsigned int);

    friend std::ostream& operator<<(std::ostream &os, const PlayerInputEvent &o) {
        os << "PlayerInputEvent: {" << std::endl;
        os << "    playerId: " << o.playerId << std::endl;
        os << "    movingForward: " << o.movingForward << std::endl;
        os << "    movingBackward: " << o.movingBackward << std::endl;
        os << "    movingLeft: " << o.movingLeft << std::endl;
        os << "    movingRight: " << o.movingRight << std::endl;
        os << "    jumping: " << o.jumping << std::endl;
        os << "    firing: " << o.firing << std::endl;
        os << "    yaw: " << o.yaw << std::endl;
        os << "    pitch: " << o.pitch << std::endl;
        os << "}";

        return os;
    }

	virtual void updateValues(const PlayerInputEvent &e);
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
