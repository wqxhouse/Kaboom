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

    virtual void serialize(char *buf);
    virtual void deserialize(char *buf);

private:
    unsigned int entityId;

    float x;
    float y;
    float z;
};
