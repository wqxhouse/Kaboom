#pragma once

#include "EntitySystem.h"

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem(Game *game);
    ~PhysicsSystem();

    virtual void update(float timeStep);
};
