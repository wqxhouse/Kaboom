#include "EntityManager.h"

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
        return NULL;
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
    if (isEntityAlive(id)) {
        return entities[id];
    } else {
        return NULL;
    }
}

bool EntityManager::isEntityAlive(unsigned int id) {
    return entities.count(id) > 0;
}
