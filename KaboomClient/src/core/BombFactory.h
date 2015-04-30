#pragma once

class Entity;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(unsigned int id, float x, float y, float z) const;

private:
    EntityManager &entityManager;
};
