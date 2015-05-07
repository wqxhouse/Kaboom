#pragma once

#include <systems/EntitySystem.h>

class Game;

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem(Game *game);

    virtual void preprocessEntities(std::vector<Entity *> entities);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
};
