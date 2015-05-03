#include "Conversion.h"

#define PI 3.14159265359
#define deg2rad(d) ((d) * PI / 180.0)

btVector3 getViewDirection(float x, float y, float z, float yaw, float pitch) {
    btQuaternion rot0;
    btQuaternion rot1;

    rot0.setRotation(btVector3(0.0f, 0.0f, 1.0f), deg2rad(-yaw));
    rot1.setRotation(btVector3(1.0f, 0.0f, 0.0f), deg2rad(pitch));

    btQuaternion rot = rot0 * rot1; // order different from osg::Quat

    btVector3 dir = quatRotate(rot, btVector3(0.0f, 1.0f, 0.0f));
    dir.normalize();

    return dir;
}
