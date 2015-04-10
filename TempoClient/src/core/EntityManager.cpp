#include "EntityManager.h"

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
}

Entity *EntityManager::createEntity() {
    while (entities.count(nextId)) {
        ++nextId;
    }

    entities[nextId] = new Entity(nextId);
    ++nextId;
}

void EntityManager::destroyEntity(unsigned int id) {
    delete entities[id];
    entities.erase(id);
}
