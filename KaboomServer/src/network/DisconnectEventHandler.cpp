#include "DisconnectEventHandler.h"

#include <core/EntityManager.h>
#include <core/Player.h>
#include <network/DisconnectEvent.h>

#include "../components/DestroyComponent.h"
#include "../components/OwnerComponent.h"
#include "../core/Game.h"

DisconnectEventHandler::DisconnectEventHandler(Game *game)
        : game(game) {
}

void DisconnectEventHandler::handle(const Event &e) const {
    const DisconnectEvent &evt = static_cast<const DisconnectEvent &>(e);

    Player *player = game->getPlayers().at(evt.getPlayerId());
    game->getGameServer().sendDisconnectEvent(player);

    Entity *entity = player->getEntity();
    if (entity != nullptr) {
        entity->attachComponent(new DestroyComponent());
    }

    auto entities = game->getEntityManager().getEntityList();
    for (auto entity : entities) {
        auto ownerComp = entity->getComponent<OwnerComponent>();

        if (ownerComp != nullptr) {
            entity->attachComponent(new DestroyComponent());
        }
    }

    game->removePlayer(player);
}
