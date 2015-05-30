#include "PickupSpawnSystem.h"

#include "../components/PickupRespawnComponent.h"
#include "../core/Game.h"

PickupSpawnSystem::PickupSpawnSystem(Game *game)
        : game(game) {
}

void PickupSpawnSystem::process() {
    auto &configs = game->getSpawnPointConfigs();
    auto &timers = game->getPickupRequest();

    for (auto it = timers.begin(); it != timers.end();) {
        auto spawnPointName = it->first;
        auto pickupTimer = it->second;

        if (pickupTimer.isExpired()) {
            auto config = configs[spawnPointName];

            EntityType entityType = static_cast<EntityType>(config.getUint("id"));
            int amount = config.getInt("amount");
            int duration = config.getInt("duration");
            float radius = config.getFloat("radius");
            osg::Vec3 osgPos = config.getVec3("position");

            Vec3 pos = Vec3(osgPos.x(), osgPos.y(), osgPos.z());

            Entity *entity = game->getPickupFactory().createPickup(
                    entityType,
                    pos,
                    amount,
                    radius);
            //attach a special spawn component to the pickup to indicate that it will respawn over time.
            entity->attachComponent(new PickupRespawnComponent(duration, spawnPointName));

            game->addEntity(entity);
            it = timers.erase(it);
        } else {
            ++it;
        }
    }
}
