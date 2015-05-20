#pragma once

#include <vector>

#include <systems/EntitySystem.h>

class Entity;
class Game;

class EntityProcessingSystem : public EntitySystem {
public:
    EntityProcessingSystem(Game *game);

    virtual void process();

    virtual void preprocessEntities(std::vector<Entity *> entities) {}
    virtual void processEntities(std::vector<Entity *> entities);

    virtual bool checkEntity(Entity *entity) = 0;

    virtual void processEntity(Entity *entity) = 0;

protected:
    Game *game;
};
