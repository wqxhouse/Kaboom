#pragma once

#include "Event.h"

class RotationEvent : public Event {
public:
    RotationEvent(unsigned int entityId, float yaw, float pitch);
    ~RotationEvent();

    virtual EventOpcode getOpcode() const;

    const unsigned int &getEntityId() const;

    const float &getYaw() const;
    const float &getPitch() const;

private:
    unsigned int entityId;

    float yaw;
    float pitch;
};
