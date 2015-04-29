#pragma once

class Entity;
class EntityManager;

class PlayerFactory {
public:
    PlayerFactory(EntityManager &entityManager);

    Entity *createPlayer(unsigned int id, float x, float y, float z) const;

private:
    EntityManager &entityManager;
};
