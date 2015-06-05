#include "PlayerSelectionEventHandler.h"

#include <core/EntityManager.h>
#include <core/Player.h>
#include <network/PlayerSelectionEvent.h>

#include "../core/Game.h"

PlayerSelectionEventHandler::PlayerSelectionEventHandler(Game *game)
        : game(game) {
}

void PlayerSelectionEventHandler::handle(const Event &e) const {
    const PlayerSelectionEvent &evt = static_cast<const PlayerSelectionEvent &>(e);

    Player *player = game->getPlayers().at(evt.getPlayerId());
    player->setCharacterType(evt.getCharacterType());

    switch (game->getGameMode().getMatchState()) {
        case GameMode::MatchState::PRE_MATCH:
        case GameMode::MatchState::IN_PROGRESS:
        case GameMode::MatchState::POST_MATCH: {
            game->addPlayerToWorld(player);
            game->getGameServer().sendNewPlayerEnterWorldEvent(
                    player,
                    game->getPlayers(),
                    game->getEntityManager().getEntityList());
            break;
        }
    }
}
