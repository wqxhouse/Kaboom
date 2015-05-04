#pragma once

#include <string>

class CollisionController;
enum EntityType;

struct BombData {
    EntityType type;
    std::string name;
    float size;
    float mass;
    float explosionRadius;
    int cooldown;
    int launchSpeed;
    int capacity;
	float knockbackRatio;
	int knockbackDuration;
	int maxDamage;
	int minDamage;

    CollisionController *collisionController;
};
