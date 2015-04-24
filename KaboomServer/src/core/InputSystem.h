#pragma once

#include "EntitySystem.h"

class InputSystem : public EntitySystem {
public:
    InputSystem(Game *game);
    ~InputSystem();

    virtual void update(float timeStep);
};
