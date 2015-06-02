#pragma once

#include "EntityProcessingSystem.h"

// Spawns the equipped bomb in front of the entity if cooldown is ready.
class FiringSystem : public EntityProcessingSystem {
public:
    FiringSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
