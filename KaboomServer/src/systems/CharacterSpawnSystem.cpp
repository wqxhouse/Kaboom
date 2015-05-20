#include "CharacterSpawnSystem.h"

#include <algorithm>

#include <components/PlayerStatusComponent.h>
#include <components/WeaponPickupComponent.h>
#include <core/EntityManager.h>

#include "../components/DestroyComponent.h"
#include "../components/TimerComponent.h"
#include "../components/RespawnComponent.h"
#include "../components/TriggerComponent.h"
#include "../components/PhysicsComponent.h"

#include "../core/Game.h"

CharacterSpawnSystem::CharacterSpawnSystem(Game *game)
        : EntityProcessingSystem(game) {
}

bool CharacterSpawnSystem::checkEntity(Entity *entity) {
    return !entity->hasComponent<DestroyComponent>() &&
            entity->hasComponent<RespawnComponent>() &&
            entity->hasComponent<PlayerStatusComponent>();
}

void CharacterSpawnSystem::processEntity(Entity *entity) {
    auto playerStatusComp = entity->getComponent<PlayerStatusComponent>();
    auto spawnComp = entity->getComponent<RespawnComponent>();

    if (playerStatusComp->getIsAlive()) {
        return;
    }

    auto spawnPoints = game->getPlayerSpawnPointList();
    auto spawnPoint = spawnPoints.at(rand() % spawnPoints.size());

    Configuration spawnConfig = game->getSpawnPointConfigs().at(spawnPoint);
    osg::Vec3 posVec3 = spawnConfig.getVec3("position");

    game->getCharacterFactory().resetCharacter(entity, Vec3(posVec3.x(), posVec3.y(), posVec3.z()));
    game->getWorld().addRigidBody(entity->getComponent<PhysicsComponent>()->getRigidBody());
}
