#pragma once

class Entity;
class EntityManager;

class PlayerFactory {
public:
    PlayerFactory(EntityManager *entityManager);
    ~PlayerFactory();

    Entity *createPlayer() const;
    Entity *createPlayer(float x, float y, float z) const;

private:
    EntityManager *entityManager;
};
