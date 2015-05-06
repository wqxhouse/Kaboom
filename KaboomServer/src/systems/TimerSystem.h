#pragma once

#include "EntitySystem.h"

// Notifies timer handler when a timer expires.
class TimerSystem : public EntitySystem {
public:
    TimerSystem(Game *game);

    virtual void update(float timeStep);
};
