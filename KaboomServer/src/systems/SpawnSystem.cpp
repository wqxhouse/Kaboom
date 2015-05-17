#include "SpawnSystem.h"

#include <components/PlayerStatusComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/EntityManager.h>

#include "../components/TimerComponent.h"
#include "../components/SpawnComponent.h"
#include "../components/TriggerComponent.h"
#include "../components/PhysicsComponent.h"

#include "../core/Game.h"

SpawnSystem::SpawnSystem(Game *game)
        : game(game),
		  mapConfigLoader(spawnConfigMap){

	mapConfigLoader.load("data-server/map.xml");

	game->getPickupSpawnPointTimerMap().clear();
	for (auto spawnConfig : spawnConfigMap) {
		if (spawnConfig.second.getString("entity-type") == "Pickup") {
			game->getPickupSpawnPointTimerMap().insert(std::make_pair(spawnConfig.first, Timer(0)));//duration is Zero at first, so we will spawn that right away
		} else if (spawnConfig.second.getString("entity-type") == "Player") {
			game->getPlayerSpawnPointList().push_back(spawnConfig.first);
		}
	}
}

//handle spawning of weapon pickups
void SpawnSystem::preprocessEntities(std::vector<Entity *> entities) {	
	for (auto spawnPointTimerIt = game->getPickupSpawnPointTimerMap().begin(); spawnPointTimerIt != game->getPickupSpawnPointTimerMap().end();) {
		if (spawnPointTimerIt->second.isExpired()) {
			const std::string spawnPointName = spawnPointTimerIt->first;//name of the spawn we need to load
			Configuration spawnConfig = spawnConfigMap[spawnPointName];

			const EntityType entityType = static_cast<EntityType>(spawnConfig.getUint("id"));
			int amount = spawnConfig.getInt("amount");
			int duration = spawnConfig.getInt("duration");
			float radius = spawnConfig.getFloat("radius");
			osg::Vec3 posVec = osg::Vec3(spawnConfig.getVec3("position"));

            Entity* pickupEntity = game->getPickupFactory().createPickup(entityType, Vec3(posVec.x(), posVec.y(), posVec.z()), amount, radius);
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
    return entity->hasComponent<PlayerStatusComponent>() &&
		   entity->hasComponent<SpawnComponent>();
}


//Handling respawning of players
void SpawnSystem::processEntity(Entity *entity) {
	auto* playerStatusComponent = entity->getComponent<PlayerStatusComponent>();
	auto* spawnComponent = entity->getComponent<SpawnComponent>();

	if (playerStatusComponent->getIsAlive() == false ){ //respawn the player here
		std::string spawnPointName = game->getPlayerSpawnPointList()[rand() % game->getPlayerSpawnPointList().size()]; //randomly pick a spawn point spot

		Configuration spawnConfig = spawnConfigMap[spawnPointName];
		osg::Vec3 posVec3 = spawnConfig.getVec3("position");

		game->getCharacterFactory().resetCharacter(entity, Vec3(posVec3.x(), posVec3.y(), posVec3.z()));
		game->getWorld().addRigidBody(entity->getComponent<PhysicsComponent>()->getRigidBody());
	}
	
}

std::unordered_map<std::string, Configuration> & SpawnSystem::getSpawnConfigMap() {
	return spawnConfigMap;
}

