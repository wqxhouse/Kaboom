#pragma once

#include "EntityData.h"

class OldEntity {
public:
    virtual ~OldEntity() {};

    virtual EntityData *getEntityData() = 0;
};
