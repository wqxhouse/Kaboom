#pragma once

#include <systems/EntitySystem.h>

// Kills all entities below z=-100
class VoidSystem : public EntitySystem {
public:
    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);
};
