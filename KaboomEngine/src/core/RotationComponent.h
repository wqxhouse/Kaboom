#pragma once

#include <ostream>

#include "Component.h"

class RotationComponent : public Component {
public:
    RotationComponent(float yaw = 0.0f, float pitch = 0.0f);

    float getYaw() const;
    float getPitch() const;

    void setYaw(float yaw);
    void setPitch(float pitch);

    void setRotation(float yaw, float pitch);

    friend std::ostream &operator<<(std::ostream &os, const RotationComponent &o) {
        os << "RotationComponent: {" << std::endl;
        os << "    yaw: " << o.yaw << std::endl;
        os << "    pitch: " << o.pitch << std::endl;
        os << "}";

        return os;
    }

private:
    float yaw;
    float pitch;
};
