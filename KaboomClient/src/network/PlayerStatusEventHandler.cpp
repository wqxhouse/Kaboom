#include "PlayerStatusEventHandler.h"

#include <osg/MatrixTransform>

#include <Core.h>
#include <GeometryObject.h>
#include <components/PlayerStatusComponent.h>
#include <core/Entity.h>
#include <network/PlayerStatusEvent.h>

#include "../core/Game.h"

PlayerStatusEventHandler::PlayerStatusEventHandler(Game *game)
        : game(game) {
}

void PlayerStatusEventHandler::handle(const Event &e) const {
	const PlayerStatusEvent &evt = static_cast<const PlayerStatusEvent &>(e);

	Entity *player = game->getEntityManager().getEntity((game->getGameClient().getCurrentPlayerEntityId()));
	PlayerStatusComponent *playerStatusComp = player->getComponent<PlayerStatusComponent>();
	playerStatusComp->setIsKnockBacked(playerStatusComp->getIsKnockBacked());
	playerStatusComp->setIsStaggered(playerStatusComp->getIsStaggered());
	playerStatusComp->setIsDamaged(playerStatusComp->getIsDamaged());
	playerStatusComp->setIsAlive(playerStatusComp->getIsAlive());
}
