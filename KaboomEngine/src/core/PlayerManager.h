#pragma once

#include <unordered_map>

#include "Player.h"

class PlayerManager {
public:
    PlayerManager();

    unsigned int generateId();

    Player *createPlayer(Entity *entity);
    Player *createPlayer(Entity *entity, unsigned int id);

    void destroyPlayer(unsigned int id);

    Player *getPlayer(unsigned int id) const;
    Player *getPlayerByEntityId(unsigned int id) const;
    bool isPlayerAlive(unsigned int id) const;

    std::vector<Player *> getPlayerList() const;

private:
    std::unordered_map<unsigned int, Player *> players;
    std::unordered_map<unsigned int, Player *> playersByEntityId;
    unsigned int nextId;
};
