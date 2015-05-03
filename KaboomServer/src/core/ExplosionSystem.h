#pragma once

#include "EntitySystem.h"

// Damages and knockbacks nearby entities.
class ExplosionSystem : public EntitySystem {
public:
    ExplosionSystem(Game *game);

    virtual void update(float timeStep);
};
