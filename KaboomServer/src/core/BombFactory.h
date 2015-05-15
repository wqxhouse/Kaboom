#pragma once

#include <unordered_map>

class Entity;
class EntityManager;
enum EntityType;
class Vec3;

class BombFactory {
public:
    BombFactory(EntityManager &entityManager);

    Entity *createBomb(
            EntityType type,
            const Vec3 &position,
            const Vec3 &velocity) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, const Vec3 &position, const Vec3 &velocity) const;
    void createKaboomV2(Entity *entity) const;
    void createTimeBomb(Entity *entity) const;
    void createRemoteDetonator(Entity *entity) const;
};
