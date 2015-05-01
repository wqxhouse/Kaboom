#pragma once

#include "EntitySystem.h"

class ItemSystem : public EntitySystem {
public:
    ItemSystem(Game *game);

    void update(float timeStep);
};
