#pragma once

#include <math/Vec3.h>
#include <btBulletDynamicsCommon.h>

class Entity;
class EntityManager;
enum EntityType;

class JumpPadFactory {
public:
	JumpPadFactory(EntityManager &entityManager);

	Entity *createJumpPad(
            EntityType type,
			const Vec3 &boxSize,
            const Vec3 &position,
			//const Vec4 &orientation,
			const Vec3 &launchSpeedVec,
			int launchDuration) const;

private:
    EntityManager &entityManager;

	void createBase(Entity *entity, const Vec3 &boxSize, const Vec3 &position, const Vec3 &launchSpeedVec, int launchDuration) const;
    void createJumpPad(Entity *entity) const;
};
