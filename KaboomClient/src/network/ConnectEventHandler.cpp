#include "ConnectEventHandler.h"

#include <core/Player.h>
#include <network/ConnectEvent.h>

#include "../core/Game.h"

ConnectEventHandler::ConnectEventHandler(Game *game)
        : game(game) {
}

void ConnectEventHandler::handle(const Event &e) const {
    const ConnectEvent &evt = static_cast<const ConnectEvent &>(e);

    game->addPlayer(new Player(evt.getPlayerId()));
}
