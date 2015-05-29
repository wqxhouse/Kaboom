#pragma once

#include "EntityProcessingSystem.h"

// Damages and knockbacks nearby entities.
class ExplosionSystem : public EntityProcessingSystem {
public:
    ExplosionSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
