#pragma once

#include "EntityProcessingSystem.h"

class InputSystem : public EntityProcessingSystem {
public:
    InputSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
