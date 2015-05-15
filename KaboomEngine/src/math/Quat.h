#pragma once

class Quat {
public:
    float x;
    float y;
    float z;
    float w;

    Quat() : Quat(0.0f, 0.0f, 0.0f, 1.0f) {}

    Quat(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w) {
    }
};
