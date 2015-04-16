#include "EntityManager.h"

#include <stdexcept>
#include <sstream>

EntityManager::EntityManager() {
}

EntityManager::~EntityManager() {
}

Entity *EntityManager::addEntity(Entity *entity) {
    if (isEntityAlive(entity->getId())) {
        std::ostringstream error;
        error << "Unable to add entity with ID " << entity->getId() << "." << std::endl;
        error << "Entity with this ID is already alive." << std::endl;

        throw std::runtime_error(error.str());
    }

    entities[entity->getId()] = entity;

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
        error << "Unable to retrieve entity with ID " << id << "." << std::endl;
        error << "Entity with this ID is not alive." << std::endl;

        throw std::runtime_error(error.str());
    }

    return entities.at(id);
}

bool EntityManager::isEntityAlive(unsigned int id) const {
    return entities.count(id) > 0;
}

unsigned int EntityManager::generateId() {
    while (isEntityAlive(nextId)) {
        ++nextId;
    }

    return nextId++;
}
