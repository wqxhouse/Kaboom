#include "MatchStateEventHandler.h"

#include <network/MatchStateEvent.h>

#include "../core/Game.h"

MatchStateEventHandler::MatchStateEventHandler(Game *game)
        : game(game) {
}

void MatchStateEventHandler::handle(const Event &e) const {
    const MatchStateEvent &evt = static_cast<const MatchStateEvent &>(e);

    const auto startTime =
            std::chrono::high_resolution_clock::now() +
            evt.getRemainingTime() -
            evt.getDuration();

    DeathmatchMode &gameMode = game->getGameMode();
    gameMode.setMatchState(evt.getState());
    gameMode.setTimer(Timer(evt.getDuration().count(), startTime));

    // TODO: Handle match state event
    if (evt.getState() == GameMode::MatchState::POST_MATCH) {
        return;
    }
}
