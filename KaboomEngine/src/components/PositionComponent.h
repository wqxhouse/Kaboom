#pragma once

#include <ostream>

#include "Component.h"

class PositionComponent : public Component {
public:
    PositionComponent(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : x(x), y(y), z(z) {
    }

    inline float getX() const {
        return x;
    }

    inline float getY() const {
        return y;
    }

    inline float getZ() const {
        return z;
    }

    inline void setX(float x) {
        this->x = x;
    }

    inline void setY(float y) {
        this->y = y;
    }

    inline void setZ(float z) {
        this->z = z;
    }

    inline void setPosition(float x, float y, float z) {
        setX(x);
        setY(y);
        setZ(z);
    }

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
