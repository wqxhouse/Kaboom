#include "AssignEventHandler.h"

#include <network/AssignEvent.h>

#include "../core/Game.h"

AssignEventHandler::AssignEventHandler(Game *game)
        : game(game) {
}

void AssignEventHandler::handle(const Event &e) const {
    const AssignEvent &evt = static_cast<const AssignEvent &>(e);

    game->setCurrentPlayer(game->getPlayers().at(evt.getPlayerId()));
}
