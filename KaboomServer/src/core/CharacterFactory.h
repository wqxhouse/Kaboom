#pragma once

#include <math/Quat.h>
#include <math/Vec3.h>

class Entity;
enum EntityType;
class EntityManager;

class CharacterFactory {
public:
    CharacterFactory(EntityManager &entityManager);

    Entity *createCharacter(
            EntityType characterType,
            const Vec3 &position = Vec3(),
            Quat rotation = Quat()) const;

private:
    EntityManager &entityManager;

    void createBase(Entity *entity, const Vec3 &position, Quat rotation) const;
    void createDefaultCharacter(Entity *entity) const;
};
