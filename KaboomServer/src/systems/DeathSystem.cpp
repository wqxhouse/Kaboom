#include "DeathSystem.h"

#include <components/HealthComponent.h>
#include <components/PlayerComponent.h>
#include <components/PlayerStatusComponent.h>
#include <components/PositionComponent.h>
#include <core/Entity.h>

#include "../components/MessageHandlerComponent.h"
#include "../components/DestroyComponent.h"
#include "../components/DetonatorComponent.h"
#include "../components/InputComponent.h"
#include "../components/PhysicsComponent.h"
#include "../components/PlayerDeathComponent.h"
#include "../core/Game.h"

#include "../messaging/BombDropMessage.h"
#include "../messaging/BombDropMessageHandler.h"

DeathSystem::DeathSystem(Game * game)
        : EntityProcessingSystem(game) {
}

bool DeathSystem::checkEntity(Entity *entity) {
    return !entity->hasComponent<DestroyComponent>() &&
            entity->hasComponent<HealthComponent>() &&
            entity->hasComponent<PlayerStatusComponent>() &&
            entity->hasComponent<PhysicsComponent>(); //this is going to be removed, the first time the player dies.
}

void DeathSystem::processEntity(Entity *entity) {
    auto healthComp = entity->getComponent<HealthComponent>();
    auto playerStatusComp = entity->getComponent<PlayerStatusComponent>();
    auto physicComp = entity->getComponent<PhysicsComponent>();
    auto handlerComp = entity->getComponent<MessageHandlerComponent>();

    if (healthComp->getAmount() == 0) {
        playerStatusComp->setAlive(false);
        //game->getGameServer().sendPlayerStatusEvent(entity);
        auto playerComp = entity->getComponent<PlayerComponent>();

        if (playerComp != nullptr) {
            game->getGameServer().sendPlayerDeathEvent(playerComp->getPlayer());
        }

        //Do a bombDrop Here
        BombDropMessage message(game, entity);
        handlerComp->getHandler()->handle(message);

        game->getWorld().removeRigidBody(physicComp->getRigidBody());
        entity->detachComponent<PhysicsComponent>(); //just remove the physicsComponent for now, we might want to attach a spectator component, or local camera on the client
        entity->detachComponent<InputComponent>();

        entity->attachComponent(new PlayerDeathComponent());

        auto detonatorComp = entity->getComponent<DetonatorComponent>();

        if (detonatorComp != nullptr) {
            auto &bombs = detonatorComp->getBombs();

            for (auto bomb : bombs) {
                bomb->attachComponent(new DestroyComponent());
            }
        }

		entity->getComponent<PositionComponent>()->setPosition(Vec3(0, 0, -10000));
    }
}
