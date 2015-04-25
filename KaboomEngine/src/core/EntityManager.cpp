#include "EntityManager.h"

#include <stdexcept>
#include <sstream>

#include "Entity.h"
#include "CharacteristicComponent.h"

EntityManager::EntityManager()
        : nextId(0) {
}

EntityManager::~EntityManager() {
}

unsigned int EntityManager::generateId() {
    while (isEntityAlive(nextId)) {
        ++nextId;
    }

    return nextId;
}

Entity *EntityManager::createEntity() {
	// TODO: figure out the logic here if createEntity() returns NULL
    return createEntity(generateId());
}

Entity *EntityManager::createEntity(unsigned int id) {
    if (isEntityAlive(id)) {
        std::ostringstream error;
        error << "Unable to create entity with ID " << id << "." << std::endl;
        error << "Entity with this ID is already alive." << std::endl;

        // throw std::runtime_error(error.str());
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

Entity *EntityManager::getEntity(unsigned int id) const {
    if (!isEntityAlive(id)) {
        std::ostringstream error;
        error << "Unable to retrieve entity with ID " << id << "." << std::endl;
        error << "Entity with this ID is not alive." << std::endl;

        // throw std::runtime_error(error.str());
		return NULL;
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

std::vector<Entity *> EntityManager::getPlayerList() const {
	std::vector<Entity *> list;

	for (auto kv : entities) {
		CharacteristicComponent *charCom = kv.second->getComponent<CharacteristicComponent>();

        if (charCom->getType() == PLAYER) {
			list.push_back(kv.second);
		}
	}

	return list;
};

std::vector<Entity *> EntityManager::getBombList() const {
	std::vector<Entity *> list;

	for (auto kv : entities) {
        CharacteristicComponent *charCom = kv.second->getComponent<CharacteristicComponent>();

        if (charCom->getType() == BOMB) {
			list.push_back(kv.second);
		}
	}

	return list;
};
