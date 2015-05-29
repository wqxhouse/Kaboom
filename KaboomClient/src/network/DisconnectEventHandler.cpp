#include "DisconnectEventHandler.h"

#include <network/DisconnectEvent.h>

#include "../core/Game.h"

DisconnectEventHandler::DisconnectEventHandler(Game *game)
        : game(game) {
}

void DisconnectEventHandler::handle(const Event &e) const {
    const DisconnectEvent &evt = static_cast<const DisconnectEvent &>(e);

    game->removePlayer(game->getPlayers().at(evt.getPlayerId()));
}
