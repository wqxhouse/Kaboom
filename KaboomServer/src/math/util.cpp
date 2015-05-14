#include "util.h"

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

btVector3 getViewDirection(float x, float y, float z, Quat rotation) {
    btVector3 dir = quatRotate(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(0.0f, 1.0f, 0.0f));
    dir.normalize();

    return dir;
}

btVector3 getImpulseVector(btVector3 pointA, btVector3 pointB, btScalar knockBackRatio) {
	btVector3 dirVec = btVector3(pointB - pointA);
	dirVec.normalize();

	btScalar distFromExplosion = pointB.distance(pointA);

	btVector3 impulseVec = (knockBackRatio / distFromExplosion) * dirVec;
	return impulseVec;

}

float getDistance(float x1, float y1, float z1, float x2, float y2, float z2) {
    const float dx = x1 - x2;
    const float dy = y1 - y2;
    const float dz = z1 - z2;

    return sqrt(dx * dx + dy * dy + dz * dz);
}
