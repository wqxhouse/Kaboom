#pragma once

#include "EntitySystem.h"

class ExplosionSystem : public EntitySystem {
public:
    ExplosionSystem(Game *game);

    virtual void update(float timeStep);
};
