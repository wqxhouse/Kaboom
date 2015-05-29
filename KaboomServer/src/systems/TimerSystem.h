#pragma once

#include "EntityProcessingSystem.h"

// Notifies timer handler when a timer expires.
class TimerSystem : public EntityProcessingSystem {
public:
    TimerSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
