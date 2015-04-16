#pragma once

#include "Event.h"

class PositionEvent : public Event {
public:
    PositionEvent(unsigned int entityId, float x, float y, float z);
    ~PositionEvent();

    virtual EventOpcode getOpcode() const;

    const unsigned int &getEntityId() const;

    const float &getX() const;
    const float &getY() const;
    const float &getZ() const;

private:
    unsigned int entityId;

    float x;
    float y;
    float z;
};
