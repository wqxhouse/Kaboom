#pragma once

#include <vector>

class EntitySystem;
class Game;

class SystemManager {
public:
    void addSystem(EntitySystem *system);
    void processSystems(Game *game);

private:
    std::vector<EntitySystem *> systems;
};
