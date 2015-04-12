#include "EntityManager.h"

#include <stdexcept>
#include <sstream>

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
        std::ostringstream error;
        error << "Unable to create entity with ID " << id << ".";
        error << "Entity with this ID is already alive.";

        throw std::runtime_error(error.str());
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

Entity *EntityManager::getEntity(unsigned int id) const {
    if (!isEntityAlive(id)) {
        std::ostringstream error;
        error << "Unable to retrieve entity with ID " << id << ".";
        error << "Entity with this ID is not alive.";

        throw std::runtime_error(error.str());
    }

    return entities.at(id);
}

bool EntityManager::isEntityAlive(unsigned int id) const {
    return entities.count(id) > 0;
}
