#pragma once

#include <ostream>

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

    virtual void serialize(char *buf) const;
    virtual void deserialize(char *buf);

    friend std::ostream& operator<<(std::ostream &os, const PositionEvent &o) {
        os << "PositionEvent: {" << std::endl;
        os << "    entityId: " << o.entityId << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
        os << "}";

        return os;
    }

private:
    unsigned int entityId;

    float x;
    float y;
    float z;
};
