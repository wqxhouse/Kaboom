#pragma once

class Vec3 {
public:
    float x;
    float y;
    float z;

    Vec3() : Vec3(0.0f, 0.0f, 0.0f) {}

    Vec3(float x, float y, float z)
            : x(x), y(y), z(z) {
    }
};
