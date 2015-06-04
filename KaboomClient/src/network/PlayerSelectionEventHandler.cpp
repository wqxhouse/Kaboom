#include "PlayerSelectionEventHandler.h"

#include <network/PlayerSelectionEvent.h>

#include "../core/Game.h"

PlayerSelectionEventHandler::PlayerSelectionEventHandler(Game *game)
        : game(game) {
}

void PlayerSelectionEventHandler::handle(const Event &e) const {
    const PlayerSelectionEvent &evt = static_cast<const PlayerSelectionEvent &>(e);
    printf("%d\n", evt.getCharacterType());
    // TODO: Handle player selection event
}
