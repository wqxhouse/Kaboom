#include "EntityManager.h"

#include <stdexcept>
#include <sstream>

#include "Entity.h"
#include "CharacteristicComponent.h"

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
        error << "Unable to create entity with ID " << id << "." << std::endl;
        error << "Entity with this ID is already alive." << std::endl;

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
        error << "Unable to retrieve entity with ID " << id << "." << std::endl;
        error << "Entity with this ID is not alive." << std::endl;

        throw std::runtime_error(error.str());
    }

    return entities.at(id);
}

bool EntityManager::isEntityAlive(unsigned int id) const {
    return entities.count(id) > 0;
}

unsigned int EntityManager::getNextId() const{
	return nextId;
}

std::vector<Entity *> EntityManager::getEntityList() {
	std::vector<Entity *> list;
	for (auto kv : entities){
		list.push_back(kv.second);
	}
	return list;
};

std::vector<Entity *> EntityManager::getPlayerList() {
	std::vector<Entity *> list;
	for (auto kv : entities){
		CharacteristicComponent * comp = kv.second->getComponent<CharacteristicComponent>();
		if ( comp->getType() == PLAYER){
			list.push_back(kv.second);
		}
	}
	return list;
};

std::vector<Entity *> EntityManager::getBombList() {
	std::vector<Entity *> list;
	for (auto kv : entities){
		CharacteristicComponent * comp = kv.second->getComponent<CharacteristicComponent>();
		if (comp->getType() == BOMB){
			list.push_back(kv.second);
		}
	}
	return list;
};
