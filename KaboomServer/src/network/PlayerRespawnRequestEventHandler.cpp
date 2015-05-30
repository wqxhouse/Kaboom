#include "PlayerRespawnRequestEventHandler.h"

#include <components/EquipmentComponent.h>
#include <core/EntityManager.h>
#include <core/Player.h>

#include <network/PlayerRespawnRequestEvent.h>

#include "../components/PlayerDeathComponent.h"
#include "../core/Game.h"

PlayerRespawnRequestEventHandler::PlayerRespawnRequestEventHandler(Game* game)
	:game(game) {

}

void PlayerRespawnRequestEventHandler::handle(const Event &e) const {

	const PlayerRespawnRequestEvent &evt = static_cast<const PlayerRespawnRequestEvent &>(e);
	Entity *entity = game->getPlayers().at(evt.getPlayerId())->getEntity();

	if (entity != nullptr) {
		auto playerDeathComp = entity->getComponent<PlayerDeathComponent>();
		if (playerDeathComp != nullptr) {
			playerDeathComp->setIsReadyToSpawn(true);
		}
	}

}