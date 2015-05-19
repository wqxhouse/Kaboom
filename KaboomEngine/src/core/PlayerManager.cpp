#include "PlayerManager.h"

#include <iostream>
#include <sstream>

#include "Entity.h"
#include "Player.h"

PlayerManager::PlayerManager()
        : nextId(0) {
}

unsigned int PlayerManager::generateId() {
    while (isPlayerAlive(nextId)) {
        ++nextId;
    }

    return nextId;
}

Player *PlayerManager::createPlayer(Entity *entity) {
    return createPlayer(entity, generateId());
}

Player *PlayerManager::createPlayer(Entity *entity, unsigned int id) {
    if (isPlayerAlive(id)) {
        std::ostringstream error;
        error << "Unable to create player with ID " << id << "." << std::endl;
        error << "Player with this ID is already alive.";

        std::cerr << error.str() << std::endl;

        return nullptr;
    }

    Player *player = new Player(id, entity);

    players[id] = player;
    playersByEntityId[entity->getId()] = player;

    return player;
}

void PlayerManager::destroyPlayer(unsigned int id) {
    if (isPlayerAlive(id)) {
        delete players[id];
        players.erase(id);
    }
}

Player *PlayerManager::getPlayer(unsigned int id) const {
    if (!isPlayerAlive(id)) {
        std::ostringstream error;
        error << "Unable to retrieve Player with ID " << id << "." << std::endl;
        error << "Player with this ID is not alive.";

        std::cerr << error.str() << std::endl;

        return nullptr;
    }

    return players.at(id);
}

Player *PlayerManager::getPlayerByEntityId(unsigned int id) const {
    return playersByEntityId.at(id);
}

bool PlayerManager::isPlayerAlive(unsigned int id) const {
    return players.count(id) > 0;
}

std::vector<Player *> PlayerManager::getPlayerList() const {
    std::vector<Player *> list;

    for (auto kv : players) {
        list.push_back(kv.second);
    }

    return list;
};
