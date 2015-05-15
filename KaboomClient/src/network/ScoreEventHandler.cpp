#include "ScoreEventHandler.h"

#include <network/ScoreEvent.h>

#include "../core/Game.h"

ScoreEventHandler::ScoreEventHandler(Game *game)
        : game(game) {
}

void ScoreEventHandler::handle(const Event &e) const {
    const ScoreEvent &evt = static_cast<const ScoreEvent &>(e);

    // TODO: Handle score update
}
