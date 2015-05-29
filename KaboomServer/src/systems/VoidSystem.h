#pragma once

#include "EntityProcessingSystem.h"

// Kills all entities below z=-100
class VoidSystem : public EntityProcessingSystem {
public:
    VoidSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
