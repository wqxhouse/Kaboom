#pragma once

#include "EntitySystem.h"

class ExplosionSystem : public EntitySystem {
public:
    ExplosionSystem(Game *game);
    ~ExplosionSystem();

    virtual void update(float timeStep);
};
