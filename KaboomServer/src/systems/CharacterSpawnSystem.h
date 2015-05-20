#pragma once

#include "EntityProcessingSystem.h"

class CharacterSpawnSystem : public EntityProcessingSystem {
public:
    CharacterSpawnSystem(Game *game);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
