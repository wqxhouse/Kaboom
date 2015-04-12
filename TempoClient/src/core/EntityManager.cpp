#include "EntityManager.h"

#include <exception>

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
}

Entity *EntityManager::createEntity() {
    while (isEntityAlive(nextId)) {
        ++nextId;
    }

    return createEntity(nextId++);
}

Entity *EntityManager::createEntity(unsigned int id) {
    if (isEntityAlive(id)) {
        throw std::exception("Entity with this ID is alive");
    }

    Entity *entity = new Entity(id);

    entities[id] = entity;

    return entity;
}

void EntityManager::destroyEntity(unsigned int id) {
    if (isEntityAlive(id)) {
        delete entities[id];
        entities.erase(id);
    }
}

Entity *EntityManager::getEntity(unsigned int id) {
    if (!isEntityAlive(id)) {
        throw std::exception("Entity with this ID is not alive");
    }

    return entities[id];
}

bool EntityManager::isEntityAlive(unsigned int id) {
    return entities.count(id) > 0;
}
