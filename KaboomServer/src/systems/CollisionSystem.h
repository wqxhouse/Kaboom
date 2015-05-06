#pragma once

#include "EntitySystem.h"

// Notifies collision handler when a collision occurs.
class CollisionSystem : public EntitySystem {
public:
    CollisionSystem(Game *game);

    virtual void update(float timeStep);
};
