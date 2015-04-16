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

	void serialize(char * buf) {
		memcpy(buf, this, sizeof(RotationEvent));
	}

	void deserialize(char * buf) {
		memcpy(this, buf, sizeof(RotationEvent));
	}

private:
    unsigned int entityId;

    float yaw;
    float pitch;
};
