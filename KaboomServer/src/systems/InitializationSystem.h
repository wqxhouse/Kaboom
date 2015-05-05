#pragma once

#include "EntitySystem.h"

class InitializationSystem : public EntitySystem {
public:
    InitializationSystem(Game *game);

    virtual void update(float timeStep);
};
