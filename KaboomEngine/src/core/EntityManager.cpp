#include "EntityManager.h"

#include <stdexcept>
#include <sstream>

#include "Entity.h"

EntityManager::EntityManager()
        : nextId(0) {
}

EntityManager::~EntityManager() {
}

const unsigned int &EntityManager::generateId() {
    while (isEntityAlive(nextId)) {
        ++nextId;
    }

    return nextId;
}

Entity *EntityManager::createEntity(const EntityType &type) {
    return createEntity(generateId(), type);
}

Entity *EntityManager::createEntity(const unsigned int &id, const EntityType &type) {
    if (isEntityAlive(id)) {
        std::ostringstream error;
        error << "Unable to create entity with ID " << id << "." << std::endl;
        error << "Entity with this ID is already alive." << std::endl;

        // throw std::runtime_error(error.str());
		return nullptr;
    }

    Entity *entity = new Entity(id, type);

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
        error << "Unable to retrieve entity with ID " << id << "." << std::endl;
        error << "Entity with this ID is not alive." << std::endl;

        // throw std::runtime_error(error.str());
        return nullptr;
    }

    return entities.at(id);
}

bool EntityManager::isEntityAlive(unsigned int id) const {
    return entities.count(id) > 0;
}

std::vector<Entity *> EntityManager::getEntityList() const {
	std::vector<Entity *> list;

	for (auto kv : entities){
		list.push_back(kv.second);
	}

	return list;
};
