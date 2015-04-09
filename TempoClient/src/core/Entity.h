#pragma once

#include "EntityData.h"

class Entity {
public:
    virtual ~Entity() {};

    virtual EntityData *getEntityData() = 0;
};
