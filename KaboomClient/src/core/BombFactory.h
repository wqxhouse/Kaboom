#pragma once

#include <math/Quat.h>

class Entity;
enum EntityType;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            unsigned int id,
            EntityType type,
            float x,
            float y,
            float z,
            Quat rotation) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, float x, float y, float z, Quat rotation) const;
    void createKaboomV2(Entity *entity) const;
    void createTimeBomb(Entity *entity) const;
    void createRemoteDetonator(Entity *entity) const;
};
