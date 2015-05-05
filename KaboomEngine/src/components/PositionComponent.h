#pragma once

#include <ostream>

#include "Component.h"

class PositionComponent : public Component {
public:
    PositionComponent(float x = 0.0f, float y = 0.0f, float z = 0.0f);

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    void setPosition(float x, float y, float z);

    friend std::ostream &operator<<(std::ostream &os, const PositionComponent &o) {
        os << "PositionComponent: {" << std::endl;
        os << "    x: " << o.x << std::endl;
        os << "    y: " << o.y << std::endl;
        os << "    z: " << o.z << std::endl;
        os << "}";

        return os;
    }

private:
    float x;
    float y;
    float z;
};
