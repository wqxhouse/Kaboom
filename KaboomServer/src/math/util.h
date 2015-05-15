#pragma once

#include <btBulletDynamicsCommon.h>

#include <math/Quat.h>
#include <math/Vec3.h>

Quat euler2Quat(float yaw, float pitch, float roll);

btVector3 getViewDirection(Quat rotation);

btVector3 getImpulseVector(btVector3 pointA, btVector3 pointB, btScalar knockBackRatio);

float getDistance(const Vec3 &pos1, const Vec3 &pos2);
