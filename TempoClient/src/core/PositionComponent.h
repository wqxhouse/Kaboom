#pragma once

#include "Component.h"
#include "GameStateData.h"

class PositionComponent : public Component {
public:
    PositionComponent();
    PositionComponent(float x, float y, float z);
    ~PositionComponent();

    virtual void update(const GameStateData &gameState);

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
