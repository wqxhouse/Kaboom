#pragma once

#include <vector>

class Entity;

class EntitySystem {
public:
    virtual ~EntitySystem() {}

    virtual void preprocessEntities(std::vector<Entity *> entities);
    virtual void processEntities(std::vector<Entity *> entities);

    virtual bool checkEntity(Entity *entity) = 0;

    virtual void processEntity(Entity *entity) = 0;
};
