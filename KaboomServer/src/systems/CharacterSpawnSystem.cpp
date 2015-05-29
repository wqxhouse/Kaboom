#include "CharacterSpawnSystem.h"

#include <components/PlayerComponent.h>
#include <components/PlayerStatusComponent.h>

#include "../components/DestroyComponent.h"
#include "../components/MessageHandlerComponent.h"
#include "../components/RespawnComponent.h"
#include "../components/PhysicsComponent.h"
#include "../core/Game.h"
#include "../messaging/CharacterSpawnMessage.h"
#include "../messaging/MessageHandler.h"

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

    if (playerStatusComp->isAlive()) {
        return;
    }

    auto playerComp = entity->getComponent<PlayerComponent>();

    if (playerComp != nullptr) {
        game->getGameServer().sendPlayerRespawnEvent(playerComp->getPlayer());
    }

    Vec3 spawnPoint = game->getPlayerSpawnPoint();

    game->getCharacterFactory().resetCharacter(entity, spawnPoint);
    game->getWorld().addRigidBody(entity->getComponent<PhysicsComponent>()->getRigidBody());

    auto handlerComp = entity->getComponent<MessageHandlerComponent>();
    if (handlerComp != nullptr) {
        CharacterSpawnMessage msg(game, entity, spawnPoint);
        handlerComp->getHandler()->handle(msg);
    }
}
