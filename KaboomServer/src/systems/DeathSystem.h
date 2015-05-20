#pragma once

#include "EntityProcessingSystem.h"

// Marks all player entity as dead, but not delete the entity
class DeathSystem : public EntityProcessingSystem {
public:
	DeathSystem(Game* game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
