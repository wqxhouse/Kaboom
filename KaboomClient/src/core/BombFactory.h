#pragma once

class Entity;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            unsigned int id,
            EntityType bombType,
            float x,
            float y,
            float z,
            float yaw,
            float pitch) const;

private:
    EntityManager &entityManager;
};
