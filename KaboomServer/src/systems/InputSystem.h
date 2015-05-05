#pragma once

#include "EntitySystem.h"

class InputSystem : public EntitySystem {
public:
    InputSystem(Game *game);

    virtual void update(float timeStep);
};
