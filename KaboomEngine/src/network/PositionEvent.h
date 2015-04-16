#pragma once

#include "Event.h"

class PositionEvent : public Event {
public:
	PositionEvent();
    PositionEvent(unsigned int entityId, float x, float y, float z);
    ~PositionEvent();


    const unsigned int &getEntityId() const;

    const float &getX() const;
    const float &getY() const;
    const float &getZ() const;

	void serialize(char * buf) {
		memcpy(buf, this, sizeof(PositionEvent));
	}

	void deserialize(char * buf) {
		memcpy(this, buf, sizeof(PositionEvent));
	}

private:
    unsigned int entityId;

    float x;
    float y;
    float z;
};
