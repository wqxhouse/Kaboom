#pragma once

#include "Entity.h"

class Player {
public:
    Player(Entity *entity);
    ~Player();

    float getX() const;
    float getY() const;
    float getZ() const;

private:
    Entity *entity;
};
