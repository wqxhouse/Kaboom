#pragma once

#include "EntityProcessingSystem.h"

// Notifies collision handler when a collision occurs.
class CollisionSystem : public EntityProcessingSystem {
public:
    CollisionSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
