#pragma once

#include <unordered_map>

class Entity;
enum EntityType;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            const EntityType &type,
            float x = 0.0f,
            float y = 0.0f,
            float z = 0.0f,
            float vx = 0.0f,
            float vy = 0.0f,
            float vz = 0.0f) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, float x, float y, float z, float vx, float vy, float vz) const;
    void createKaboomV2(Entity *entity) const;
    void createTimeBomb(Entity *entity) const;
};
