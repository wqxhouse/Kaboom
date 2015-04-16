#pragma once

#include "Event.h"

class RotationEvent : public Event {
public:
	RotationEvent();
    RotationEvent(unsigned int entityId, float yaw, float pitch);
    ~RotationEvent();

    const unsigned int &getEntityId() const;

    const float &getYaw() const;
    const float &getPitch() const;

    virtual void serialize(char *buf);
    virtual void deserialize(char *buf);

private:
    unsigned int entityId;

    float yaw;
    float pitch;
};
