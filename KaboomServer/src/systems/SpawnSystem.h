#pragma once

#include <systems/EntitySystem.h>

#include "../core/MapConfigLoader.h"

class Game;

class SpawnSystem : public EntitySystem {
public:
	SpawnSystem(Game *game);

	virtual void preprocessEntities(std::vector<Entity *> entities);

    virtual bool checkEntity(Entity *entity);

    virtual void processEntity(Entity *entity);

	std::unordered_map<std::string, Configuration> & getSpawnConfigMap();

private:
    Game *game;
	MapConfigLoader mapConfigLoader;

	std::unordered_map<std::string, Configuration> spawnConfigMap;

};
