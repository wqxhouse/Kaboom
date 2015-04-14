#pragma once

#include <core/Entity.h>

class Player : public Entity {
public:
    Player(unsigned int id);
    ~Player();

    float getX() const;
    float getY() const;
    float getZ() const;

    void setPosition(float x, float y, float z);
};
