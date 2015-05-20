#pragma once

#include "EntityProcessingSystem.h"

class PickupSystem : public EntityProcessingSystem {
public:
    PickupSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
