#pragma once

#include <unordered_map>

#include "Entity.h"

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    Entity *addEntity(Entity *entity);

    void destroyEntity(unsigned int id);

    Entity *getEntity(unsigned int id) const;

    bool isEntityAlive(unsigned int id) const;

    unsigned int generateId();

private:
    std::unordered_map<unsigned int, Entity *> entities;
    unsigned int nextId;

};
