#pragma once

#include "Entity.h"

class Player : public Entity {
public:
    Player(unsigned int id);
    ~Player();

    float getX() const;
    float getY() const;
    float getZ() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);

    void setPosition(float x, float y, float z);

    float getYaw() const;
    float getPitch() const;

    void setYaw(float yaw);
    void setPitch(float pitch);

    void setRotation(float yaw, float pitch);
};
