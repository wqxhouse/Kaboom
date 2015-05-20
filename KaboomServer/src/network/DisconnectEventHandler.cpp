#include "DisconnectEventHandler.h"

#include <components/BombContainerComponent.h>
#include <core/EntityManager.h>
#include <network/DisconnectEvent.h>

#include "../components/DestroyComponent.h"
#include "../components/DetonatorComponent.h"
#include "../components/OwnerComponent.h"
#include "../core/Game.h"

DisconnectEventHandler::DisconnectEventHandler(Game *game)
        : game(game) {
}

void DisconnectEventHandler::handle(const Event &e) const {
    const DisconnectEvent &evt = static_cast<const DisconnectEvent &>(e);

    EntityManager &entityManager = game->getEntityManager();
    Entity *player = entityManager.getEntity(evt.getPlayerId());

    game->getGameServer().sendDisconnectEvent(player);

    player->attachComponent(new DestroyComponent());

    auto entities = game->getEntityManager().getEntityList();

    for (auto entity : entities) {
        auto ownerComp = entity->getComponent<OwnerComponent>();

        if (ownerComp != nullptr) {
            entity->attachComponent(new DestroyComponent());
        }
    }
}
