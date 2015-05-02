#pragma once

class Entity;
class EntityManager;

class PlayerFactory {
public:
    PlayerFactory(EntityManager &entityManager);

    Entity *createPlayer(float x = 0.0f, float y = 0.0f, float z = 0.0f) const;

private:
    EntityManager &entityManager;
};
