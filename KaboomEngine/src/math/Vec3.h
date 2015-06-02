#pragma once

#include <osg/Vec3>

class Vec3 {
public:
    float x;
    float y;
    float z;

    Vec3() : Vec3(0.0f, 0.0f, 0.0f) {}

    Vec3(float x, float y, float z)
            : x(x), y(y), z(z) {
    }

    inline osg::Vec3 getOsgVec3() const {
        return osg::Vec3(x, y, z);
    }

    inline void setOsgVec3(const osg::Vec3 &v) {
        x = v.x();
        y = v.y();
        z = v.z();
    }
};
