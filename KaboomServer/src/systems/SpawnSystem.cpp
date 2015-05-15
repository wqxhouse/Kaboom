#include "SpawnSystem.h"

#include <components/PlayerStatusComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/EntityManager.h>

#include "../components/TimerComponent.h"
#include "../components/SpawnComponent.h"
#include "../components/TriggerComponent.h"

#include "../core/Game.h"

SpawnSystem::SpawnSystem(Game *game)
        : game(game),
		  mapConfigLoader(spawnConfigMap){

	mapConfigLoader.load("data-server/map.xml");

	game->getPickupSpawnPointTimerMap().clear();
	for (auto spawnConfig : spawnConfigMap) {
		game->getPickupSpawnPointTimerMap().insert(std::make_pair(spawnConfig.first, Timer(0)));//duration is Zero at first, so we will spawn that right away
	}
}

void SpawnSystem::preprocessEntities(std::vector<Entity *> entities) {
	
	//handle spawning and respawning of pickups
	for (auto spawnPointTimerIt = game->getPickupSpawnPointTimerMap().begin(); spawnPointTimerIt != game->getPickupSpawnPointTimerMap().end();) {
		if (spawnPointTimerIt->second.isExpired()) {
			const std::string spawnPointName = spawnPointTimerIt->first;//name of the spawn we need to load
			Configuration spawnConfig = spawnConfigMap[spawnPointName];

			const EntityType entityType = static_cast<EntityType>(spawnConfig.getUint("id"));
			int amount = spawnConfig.getInt("amount");
			int duration = spawnConfig.getInt("duration");
			float radius = spawnConfig.getFloat("radius");
			osg::Vec3 posVec = osg::Vec3(spawnConfig.getVec3("position"));

			Entity* pickupEntity = game->getPickupFactory().createPickup(entityType, amount, radius, posVec.x(), posVec.y(), posVec.z());
			//attach a special spawn component to the pickup to indicate that it will respawn over time, and contain information about the spawn
			pickupEntity->attachComponent(new SpawnComponent(duration, spawnPointName));
			game->addEntity(pickupEntity);

			spawnPointTimerIt = game->getPickupSpawnPointTimerMap().erase(spawnPointTimerIt); //remove it from the map once we have finish the request
		} else {
			spawnPointTimerIt++;
		}
	}
}

bool SpawnSystem::checkEntity(Entity *entity) {
    return entity->hasComponent<PlayerStatusComponent>();
}


//for pickups, a global list of Timer, with string maps to timer
void SpawnSystem::processEntity(Entity *entity) {
	auto* playerStatusComponent = entity->getComponent<PlayerStatusComponent>();
	
	if (playerStatusComponent->getIsAlive() == false){ //respawn the player here

		
	}
	
}

std::unordered_map<std::string, Configuration> & SpawnSystem::getSpawnConfigMap() {
	return spawnConfigMap;
}

