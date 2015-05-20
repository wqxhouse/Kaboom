#pragma once

#include <math/Quat.h>

class Entity;
enum EntityType;
class EntityManager;
class Vec3;

class JumpPadFactory {
public:
	JumpPadFactory(EntityManager &entityManager);

    Entity *createJumpPad(
            unsigned int id,
            EntityType jumpPadType,
            const Vec3 &position,
            Quat rotation) const;

private:
    EntityManager &entityManager;
};
