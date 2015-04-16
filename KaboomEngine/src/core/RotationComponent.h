#pragma once

#include "Component.h"

class RotationComponent : public Component {
public:
    RotationComponent();
    RotationComponent(float yaw, float pitch);
    ~RotationComponent();

    float getYaw() const;
    float getPitch() const;

    void setYaw(float yaw);
    void setPitch(float pitch);

    void setRotation(float yaw, float pitch);

private:
    float yaw;
    float pitch;
};
