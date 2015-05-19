#include "DestroyEventHandler.h"

#include <network/DestroyEvent.h>

#include "../core/Game.h"

DestroyEventHandler::DestroyEventHandler(Game *game)
        : game(game) {
}

void DestroyEventHandler::handle(const Event &e) const {
    const DestroyEvent &evt = static_cast<const DestroyEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    if (entity != nullptr) {
        game->removeEntity(entity);
    }
}
