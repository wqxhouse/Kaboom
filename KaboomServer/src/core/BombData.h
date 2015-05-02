#pragma once

#include <string>

struct BombData {
    unsigned int id;
    std::string name;
    float size;
    float mass;
    float explosionRadius;
    int cooldown;
    int launchSpeed;
};
