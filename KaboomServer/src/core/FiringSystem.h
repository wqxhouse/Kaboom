#pragma once

#include "EntitySystem.h"

class FiringSystem : public EntitySystem {
public:
	FiringSystem(Game *game);

    virtual void update(float timeStep);
};
