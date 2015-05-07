#pragma once

#include <systems/EntitySystem.h>

class Game;
class World;

class PhysicsSystem : public EntitySystem {
public:
    PhysicsSystem(Game *game, World &world);

    virtual void preprocessEntities(std::vector<Entity *> entities);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

private:
    Game *game;
    World &world;
};
