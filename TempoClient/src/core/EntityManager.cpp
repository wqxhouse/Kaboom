#include "EntityManager.h"

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
}

Entity *EntityManager::createEntity() {
    while (entities.count(nextId)) {
        ++nextId;
    }

    Entity *entity = new Entity(nextId);

    entities[nextId] = entity;
    ++nextId;

    return entity;
}

void EntityManager::destroyEntity(unsigned int id) {
    delete entities[id];
    entities.erase(id);
}
