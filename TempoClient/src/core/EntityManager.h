#pragma once

#include <unordered_map>

#include "Entity.h"

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    Entity *createEntity();
    void destroyEntity(unsigned int id);

private:
    std::unordered_map<unsigned int, Entity *> entities;
    unsigned int nextId;
};
