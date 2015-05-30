#include "PlayerRespawnRequestEventHandler.h"

#include <components/EquipmentComponent.h>
#include <core/EntityManager.h>
#include <core/Player.h>

#include <network/PlayerRespawnRequestEvent.h>

#include "../components/PlayerRespawnComponent.h"
#include "../core/Game.h"

PlayerRespawnRequestEventHandler::PlayerRespawnRequestEventHandler(Game* game)
	:game(game) {

}

void PlayerRespawnRequestEventHandler::handle(const Event &e) const {

	const PlayerRespawnRequestEvent &evt = static_cast<const PlayerRespawnRequestEvent &>(e);
	Entity *entity = game->getPlayers().at(evt.getPlayerId())->getEntity();

	if (entity != nullptr) {
		auto playerRespawnComp = entity->getComponent<PlayerRespawnComponent>();
		if (playerRespawnComp != nullptr) {
			playerRespawnComp->setIsReadyToSpawn(true);
		}
	}

}