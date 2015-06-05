#include "VoidSystem.h"

#include <components/HealthComponent.h>
#include <components/PositionComponent.h>
#include <core/Entity.h>
#include <core/Player.h>

#include "../components/PlayerDeathComponent.h"
#include "../components/DestroyComponent.h"
#include "../components/OwnerComponent.h"
#include "../core/Game.h"

VoidSystem::VoidSystem(Game *game)
        : EntityProcessingSystem(game) {
}

bool VoidSystem::checkEntity(Entity *entity) {
    return !entity->hasComponent<PlayerDeathComponent>() &&
		  entity->hasComponent<PositionComponent>();
}

void VoidSystem::processEntity(Entity *entity) {
    auto posComp = entity->getComponent<PositionComponent>();
    auto healthComp = entity->getComponent<HealthComponent>();

    if (posComp->getPosition().z < -100) { // TODO: Extract constant to XML
        if (healthComp != nullptr) {
            healthComp->setAmount(0);

            for (auto kv : game->getPlayers()) {
                const auto player = kv.second;
                const Entity *playerEntity = player->getEntity();

                if (playerEntity->getId() == entity->getId()) {
                    player->setDeaths(player->getDeaths() + 1);
                    game->getGameServer().sendChatEvent(player->getName() + " fell out of the map.");
                    game->getGameServer().sendScoreEvent(player);
                }
            }
        } else {
            entity->attachComponent(new DestroyComponent());
        }
    }
}
