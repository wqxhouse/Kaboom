#pragma once

#include <string>

class CollisionHandler;
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
	float knockbackAmount;
	int knockbackDuration;
	int maxDamage;
	int minDamage;

    CollisionHandler *collisionHandler;
};
