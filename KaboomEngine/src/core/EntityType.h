#pragma once

#include <unordered_map>

enum EntityCategory {
    CAT_PLAYER = 1 << 16,
    CAT_BOMB = 2 << 16,
    CAT_PICKUP = 3 << 16
};

enum EntityType {
    NONE = 0,
    PLAYER = CAT_PLAYER | 1,
    KABOOM_V2 = CAT_BOMB | 1,
};

//std::unordered_map<unsigned int, EntityType> entityTypeLookup;
