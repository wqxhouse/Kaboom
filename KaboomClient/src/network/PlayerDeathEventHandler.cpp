#include "PlayerDeathEventHandler.h"

#include <network/PlayerDeathEvent.h>

#include "../core/Game.h"

PlayerDeathEventHandler::PlayerDeathEventHandler(Game *game)
        : game(game) {
}

void PlayerDeathEventHandler::handle(const Event &e) const {
    const PlayerDeathEvent &evt = static_cast<const PlayerDeathEvent &>(e);

    if (game->getCurrentPlayer()->getId() == evt.getPlayerId()) {
        game->playerAlive = false;
        game->voiceSource->setSound(game->voiceMap->at(Game::VoiceActing::DEATH_1));
        game->deathTime = std::chrono::high_resolution_clock::now();
        game->voiceSource->setGain(1);
        game->voiceSource->play();
        game->getGameGUIEventHandler()->handle(evt);
    }
}
