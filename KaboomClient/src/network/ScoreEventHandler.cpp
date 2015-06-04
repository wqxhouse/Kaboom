#include "ScoreEventHandler.h"

#include <network/ScoreEvent.h>

#include "../core/Game.h"

ScoreEventHandler::ScoreEventHandler(Game *game)
        : game(game) {
}

void ScoreEventHandler::handle(const Event &e) const {
    const ScoreEvent &evt = static_cast<const ScoreEvent &>(e);

    Player *player = game->getPlayers().at(evt.getPlayerId());
	if (game->getPlayers().at(evt.getPlayerId()) == game->getCurrentPlayer()){
		if (player->getKills() < evt.getKills()){
			game->voiceSource->setSound(game->voiceMap->at(Game::VoiceActing::KILL_1));
			game->voiceSource->setGain(1);
			game->voiceSource->play();
		}
	}
    player->setKills(evt.getKills());
    player->setDeaths(evt.getDeaths());
	if (player == game->getCurrentPlayer()){
		game->getGameGUIEventHandler()->hudScore(evt);
	}
	game->getGameGUIEventHandler()->handle(evt, player->getName());
}
