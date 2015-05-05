#pragma once

#include "EntitySystem.h"

class PickupSystem : public EntitySystem {
public:
    PickupSystem(Game *game);

    virtual void update(float timeStep);
};
