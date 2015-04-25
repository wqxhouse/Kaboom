#pragma once

#include <unordered_map>

#include "Entity.h"

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    unsigned int generateId();

    Entity *createEntity();
    Entity *createEntity(unsigned int id);

    void destroyEntity(unsigned int id);

    Entity *getEntity(unsigned int id) const;
    bool isEntityAlive(unsigned int id) const;

    std::vector<Entity *> getEntityList() const;
    std::vector<Entity *> getPlayerList() const;
    std::vector<Entity *> getBombList() const;

private:
    std::unordered_map<unsigned int, Entity *> entities;
    unsigned int nextId;
};
