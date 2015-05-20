#pragma once

#include "EntityProcessingSystem.h"

class DestroySystem : public EntityProcessingSystem {
public:
    DestroySystem(Game* game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
