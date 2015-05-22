#include "BindEventHandler.h"

#include <network/BindEvent.h>

#include "../core/Game.h"

BindEventHandler::BindEventHandler(Game *game)
        : game(game) {
}

void BindEventHandler::handle(const Event &e) const {
    const BindEvent &evt = static_cast<const BindEvent &>(e);

    Player *player = game->getPlayers().at(evt.getPlayerId());
    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    player->setEntity(entity);
}
