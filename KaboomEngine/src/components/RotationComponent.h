#pragma once

#include <ostream>

#include "Component.h"
#include "../math/Quat.h"

class RotationComponent : public Component {
public:
    RotationComponent(Quat rotation = Quat()) {}

    inline Quat getRotation() const {
        return rotation;
    }

    inline void setRotation(Quat rotation) {
        this->rotation = rotation;
    }

    friend std::ostream &operator<<(std::ostream &os, const RotationComponent &o) {
        os << "RotationComponent: {" << std::endl;
        os << "    x: " << o.rotation.x << std::endl;
        os << "    y: " << o.rotation.y << std::endl;
        os << "    z: " << o.rotation.z << std::endl;
        os << "    w: " << o.rotation.w << std::endl;
        os << "}";

        return os;
    }

private:
    Quat rotation;
};
