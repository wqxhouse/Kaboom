#pragma once

#include <unordered_map>

#include <math/Vec3.h>

class Entity;
enum EntityType;
class EntityManager;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            EntityType type,
            const Vec3 &position,
            const Vec3 &rotation) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, const Vec3 &position, const Vec3 &rotation) const;
    void createKaboomV2(Entity *entity) const;
    void createTimeBomb(Entity *entity) const;
    void createRemoteDetonator(Entity *entity) const;
};
