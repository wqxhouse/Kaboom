#pragma once

#include <string>

#include "EntityType.h"

class Entity;

class Player {
public:
    Player(unsigned int id,
            const std::string &name = "Player",
            EntityType characterType = BLUE_CHARACTER,
            Entity *entity = nullptr,
            int kills = 0,
            int deaths = 0)
            : id(id),
              name(name),
              characterType(characterType),
              entity(entity),
              kills(kills),
              deaths(deaths) {
    }

    inline unsigned int getId() const {
        return id;
    }

    inline const std::string &getName() const {
        return name;
    }

    inline void setName(const std::string &name) {
        this->name = name;
    }

    inline EntityType getCharacterType() const {
        return characterType;
    }

    inline void setCharacterType(EntityType characterType) {
        this->characterType = characterType;
    }

    inline Entity *getEntity() const {
        return entity;
    }

    inline void setEntity(Entity *entity) {
        this->entity = entity;
    }

    inline int getKills() const {
        return kills;
    }

    inline int getDeaths() const {
        return deaths;
    }

    inline void setKills(int kills) {
        this->kills = kills;
    }

    inline void setDeaths(int deaths) {
        this->deaths = deaths;
    }

private:
    unsigned int id;
    std::string name;
    EntityType characterType;

    Entity *entity;

    int kills;
    int deaths;
};
