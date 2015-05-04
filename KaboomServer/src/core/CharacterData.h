#pragma once

#include <string>
#include <unordered_map>

struct CharacterData {
    unsigned int id;
    std::string name;
    float mass;
    std::unordered_map<EntityType, int> inventory;
	int healthStartAmount;
	int healthCap;
};
