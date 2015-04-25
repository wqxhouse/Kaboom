#include "DisconnectEventHandler.h"

#include <network/DisconnectEvent.h>

#include "../core/Game.h"

DisconnectEventHandler::DisconnectEventHandler(Game *game)
        : game(game) {
}

DisconnectEventHandler::~DisconnectEventHandler() {
}

void DisconnectEventHandler::handle(const Event &e) const {
    const DisconnectEvent &evt = static_cast<const DisconnectEvent &>(e);

    EntityManager &entityManager = game->getEntityManager();
    Entity *player = entityManager.getEntity(evt.getPlayerId());

    game->getGeometryManager()->deleteGeometry(std::to_string(evt.getPlayerId()));

    entityManager.destroyEntity(player->getId());
}
