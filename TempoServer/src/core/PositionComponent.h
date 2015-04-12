#pragma once

#include "Component.h"

class PositionComponent : public Component {
public:
    PositionComponent();
    PositionComponent(float x, float y, float z);
    ~PositionComponent();

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(const float &x);
    void setY(const float &y);
    void setZ(const float &z);

private:
    float x;
    float y;
    float z;
};
