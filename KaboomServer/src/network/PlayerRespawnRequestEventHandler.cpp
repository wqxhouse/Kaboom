
#include <network/PlayerSpawnRequestEvent.h>
#include <core/EntityManager.h>
#include <core/Player.h>

#include "PlayerRespawnRequestEventHandler.h"

PlayerRespawnRequestEventHandler::PlayerRespawnRequestEventHandler(Game* game)
	: game(game){
}


PlayerRespawnRequestEventHandler::~PlayerRespawnRequestEventHandler()
{
}

void PlayerRespawnRequestEventHandler::handle(const Event &e) const {
	const PlayerRespawnRequestEvent &evt = static_cast<const PlayerRespawnRequestEvent &>(e);

	if ()
	///Player *player = game->getPlayers().at(evt.getPlayerId());

	//if (player != nullptr){
		
	//}
}


