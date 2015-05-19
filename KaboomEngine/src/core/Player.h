#pragma once

class Entity;

class Player {
public:
    Player(unsigned int id, Entity *entity, int kills = 0, int deaths = 0)
            : id(id),
              entity(entity),
              kills(kills),
              deaths(deaths) {
    }

    inline unsigned int getId() const {
        return id;
    }

    inline Entity *getEntity() const {
        return entity;
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
    Entity *entity;

    int kills;
    int deaths;
};
