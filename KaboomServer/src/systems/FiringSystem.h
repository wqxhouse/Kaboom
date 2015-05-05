#pragma once

#include "EntitySystem.h"

// Spawns the equipped bomb in front of the entity if cooldown is ready.
class FiringSystem : public EntitySystem {
public:
	FiringSystem(Game *game);

    virtual void update(float timeStep);
};
