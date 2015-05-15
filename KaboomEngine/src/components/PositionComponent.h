#pragma once

#include <ostream>

#include "Component.h"
#include "../math/Vec3.h"

class PositionComponent : public Component {
public:
    PositionComponent(const Vec3 &position)
            : position(position) {
    }

    inline const Vec3 &getPosition() const {
        return position;
    }

    inline void setPosition(const Vec3 &position) {
        this->position = position;
    }

    friend std::ostream &operator<<(std::ostream &os, const PositionComponent &o) {
        os << "PositionComponent: {" << std::endl;
        os << "    x: " << o.position.x << std::endl;
        os << "    y: " << o.position.y << std::endl;
        os << "    z: " << o.position.z << std::endl;
        os << "}";

        return os;
    }

private:
    Vec3 position;
};
