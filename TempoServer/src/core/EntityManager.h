#pragma once

#include <unordered_map>

#include "Entity.h"

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    Entity *createEntity();
    Entity *createEntity(unsigned int id);

    void destroyEntity(unsigned int id);

    Entity *getEntity(unsigned int id);
    bool isEntityAlive(unsigned int id);

private:
    std::unordered_map<unsigned int, Entity *> entities;
    unsigned int nextId;
};
