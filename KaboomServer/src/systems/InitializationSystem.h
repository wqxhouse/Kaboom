#pragma once

#include "EntityProcessingSystem.h"

class InitializationSystem : public EntityProcessingSystem {
public:
    InitializationSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
