#include "MatchStateEventHandler.h"

#include <network/MatchStateEvent.h>

#include "../core/Game.h"

MatchStateEventHandler::MatchStateEventHandler(Game *game)
        : game(game) {
}

void MatchStateEventHandler::handle(const Event &e) const {
    const MatchStateEvent &evt = static_cast<const MatchStateEvent &>(e);

    // TODO: Handle match state event
}
