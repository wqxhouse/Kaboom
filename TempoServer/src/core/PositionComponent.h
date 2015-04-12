#pragma once

#include "Component.h"

class PositionComponent : public Component {
public:
    PositionComponent();
    PositionComponent(float x, float y, float z);
    ~PositionComponent();

    virtual void update(float timeStep);

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);

private:
    float x;
    float y;
    float z;
};
