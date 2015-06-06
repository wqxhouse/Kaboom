#include "PlayerRespawnEventHandler.h"

#include <network/PlayerRespawnEvent.h>

#include "../core/Game.h"

PlayerRespawnEventHandler::PlayerRespawnEventHandler(Game *game)
        : game(game) {
}

void PlayerRespawnEventHandler::handle(const Event &e) const {
    const PlayerRespawnEvent &evt = static_cast<const PlayerRespawnEvent &>(e);

    // TODO: Handle player respawn event
	game->voiceState->setSample(game->voiceMap->at(Game::VoiceActing::DEATH_1));
	//game->voiceState->setPosition(osg::Vec3(pos.x, pos.y, pos.z));
	game->voiceState->setPlay(true);
	game->justDied = false;
	game->playerAlive = true;
	game->getGameGUIEventHandler()->handle(evt);
}
