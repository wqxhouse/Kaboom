#pragma once

#include "EntityProcessingSystem.h"

class World;

class PhysicsSystem : public EntityProcessingSystem {
public:
    PhysicsSystem(Game *game, World &world);

    virtual void preprocessEntities(std::vector<Entity *> entities);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    World &world;
};
