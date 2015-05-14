#include "SpawnSystem.h"

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
    return true;
}


//for pickups, a global list of Timer, with string maps to timer
void SpawnSystem::processEntity(Entity *entity) {

	////For weapon pickup
	//WeaponPickupComponent* weaponPickupComp = entity->getComponent<WeaponPickupComponent>();
	//TimerComponent* timerComp = entity->getComponent<TimerComponent>();
	//SpawnComponent* spawnComp = entity->getComponent<SpawnComponent>();

	//if (weaponPickupComp == nullptr || timerComp == nullptr || spawnComp == nullptr){
	//	return;
	//}

	////now check if the component is ready to spawn
	//if (timerComp->getTimer()->isExpired()){
	//	const float PICKUP_RADIUS = 1.0f; // TODO: Extract this to XML

	//	btTransform worldTrans;
	//	worldTrans.setIdentity();
	//	worldTrans.setOrigin(spawnComp->getSpawnLocationVec()); //we reuse the same spot here

	//	btGhostObject *ghostObject = new btGhostObject();
	//	ghostObject->setCollisionShape(new btSphereShape(PICKUP_RADIUS));
	//	ghostObject->setUserPointer(entity);
	//	ghostObject->setWorldTransform(worldTrans);

	//	//place the new weapon pickup here
	//	entity->attachComponent(new TriggerComponent(ghostObject));

	//	entity->detachComponent<TimerComponent>();
	//}
}

std::unordered_map<std::string, Configuration> & SpawnSystem::getSpawnConfigMap() {
	return spawnConfigMap;
}

