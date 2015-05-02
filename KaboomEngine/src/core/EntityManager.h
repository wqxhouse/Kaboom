#pragma once

#include <unordered_map>

#include "Entity.h"

class EntityManager {
public:
    EntityManager();

    unsigned int generateId();

    Entity *createEntity(const EntityType &type = NONE);
    Entity *createEntity(unsigned int id, const EntityType &type = NONE);

    void destroyEntity(unsigned int id);

    Entity *getEntity(unsigned int id) const;
    bool isEntityAlive(unsigned int id) const;

    std::vector<Entity *> getEntityList() const;

private:
    std::unordered_map<unsigned int, Entity *> entities;
    unsigned int nextId;
};
