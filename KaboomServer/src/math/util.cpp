#include "util.h"

#include <osg/Matrix>

#define PI 3.14159265359
#define deg2rad(d) ((d) * PI / 180.0)

Quat euler2Quat(float yaw, float pitch, float roll) {
    btQuaternion rot0;
    btQuaternion rot1;

    rot0.setRotation(btVector3(0.0f, 0.0f, 1.0f), deg2rad(-yaw));
    rot1.setRotation(btVector3(1.0f, 0.0f, 0.0f), deg2rad(pitch));

    btQuaternion rot = rot0 * rot1; // order different from osg::Quat

    return Quat(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
}

btVector3 getViewDirection(Quat rotation) {
    btVector3 dir = quatRotate(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(0.0f, 1.0f, 0.0f));
    dir.normalize();

    return dir;
}

Vec3 rotateVector(const Vec3 &vec, const Vec3 &axis, float deg) {
    osg::Matrix rotMat;
    rotMat.makeRotate(deg2rad(deg), axis.getOsgVec3());
    osg::Vec3 result = rotMat * vec.getOsgVec3();

    return Vec3(result.x(), result.y(), result.z());
}

btVector3 getImpulseVector(btVector3 pointA, btVector3 pointB, btScalar knockBackRatio) {
	btVector3 dirVec = btVector3(pointB - pointA);
	dirVec.normalize();

	btScalar distFromExplosion = pointB.distance(pointA);

	btVector3 impulseVec = (knockBackRatio / distFromExplosion) * dirVec;
	return impulseVec;

}

float getDistance(const Vec3 &pos1, const Vec3 &pos2) {
    const float dx = pos1.x - pos2.x;
    const float dy = pos1.y - pos2.y;
    const float dz = pos1.z - pos2.z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}
