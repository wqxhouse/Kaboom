#include "SpawnSystem.h"

#include <components/PlayerStatusComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/EntityManager.h>

#include "../components/DestroyComponent.h"
#include "../components/TimerComponent.h"
#include "../components/RespawnComponent.h"
#include "../components/TriggerComponent.h"
#include "../components/PhysicsComponent.h"

#include "../core/Game.h"

SpawnSystem::SpawnSystem(Game *game)
        : game(game){
}

//handle spawning of weapon pickups
void SpawnSystem::preprocessEntities(std::vector<Entity *> entities) {	
	//loop through the pickup spawn point and spawn them
	for (auto spawnPointTimerIt = game->getPickupRequest().begin(); spawnPointTimerIt != game->getPickupRequest().end();) {
		//check if the timer has expired, if so we want to spawn that pickup
		if (spawnPointTimerIt->second.isExpired()) {
			const std::string spawnPointName = spawnPointTimerIt->first;
			Configuration spawnConfig = game->getMapConfigMap()[spawnPointName];

			const EntityType entityType = static_cast<EntityType>(spawnConfig.getUint("id"));
			int amount = spawnConfig.getInt("amount");
			int duration = spawnConfig.getInt("duration");
			float radius = spawnConfig.getFloat("radius");
			osg::Vec3 posVec = osg::Vec3(spawnConfig.getVec3("position"));

            Entity* pickupEntity = game->getPickupFactory().createPickup(entityType, Vec3(posVec.x(), posVec.y(), posVec.z()), amount, radius);
			//attach a special spawn component to the pickup to indicate that it will respawn over time.
			pickupEntity->attachComponent(new RespawnComponent(duration, spawnPointName));
			game->addEntity(pickupEntity);

			//remove it from the map once we have finish the request
			spawnPointTimerIt = game->getPickupRequest().erase(spawnPointTimerIt);
		} else {
			spawnPointTimerIt++;
		}
	}
}

bool SpawnSystem::checkEntity(Entity *entity) {
    return !entity->hasComponent<DestroyComponent>() && 
            entity->hasComponent<PlayerStatusComponent>() &&
			entity->hasComponent<RespawnComponent>();
}


//Handling respawning of players
void SpawnSystem::processEntity(Entity *entity) {
	auto* playerStatusComponent = entity->getComponent<PlayerStatusComponent>();
	auto* respawnComponent = entity->getComponent<RespawnComponent>();

	if (playerStatusComponent->getIsAlive() == false ){ //respawn the player here
		std::string spawnPointName = game->getPlayerSpawnPointList()[rand() % game->getPlayerSpawnPointList().size()]; //randomly pick a spawn point spot

		Configuration spawnConfig = game->getMapConfigMap()[spawnPointName];
		osg::Vec3 posVec3 = spawnConfig.getVec3("position");

		game->getCharacterFactory().resetCharacter(entity, Vec3(posVec3.x(), posVec3.y(), posVec3.z()));
		game->getWorld().addRigidBody(entity->getComponent<PhysicsComponent>()->getRigidBody());
	}
	
}
