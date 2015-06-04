#include "MatchStateEventHandler.h"

#include <network/MatchStateEvent.h>
#include <network/PlayerRespawnEvent.h>

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
	if (evt.getState() == GameMode::MatchState::PRE_MATCH) {
		if (!game->playerAlive){
			PlayerRespawnEvent player;
			game->playerAlive = true;
			game->getGameGUIEventHandler()->handle(player);
		}
		game->getGameGUIEventHandler()->setPreGame();
		return;
	}
	if (evt.getState() == GameMode::MatchState::IN_PROGRESS) {
		game->getGameGUIEventHandler()->inProgress();
		return;
	}
    if (evt.getState() == GameMode::MatchState::POST_MATCH) {
		int max=game->getGameGUIEventHandler()->postGame();
		if (game->getCurrentPlayer()->getKills()>=max){
			game->voiceSource->setSound(game->voiceMap->at(Game::VoiceActing::END_GAME_VICTORY_1));
			game->voiceSource->setGain(1);
			game->voiceSource->play();
		}
		else{
			//TODO add in end game lose
		}
        return;
    }
}
