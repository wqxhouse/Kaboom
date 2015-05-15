#include "HealthEventHandler.h"

#include <osg/MatrixTransform>

#include <Core.h>
#include <GeometryObject.h>
#include <components/HealthComponent.h>
#include <core/Entity.h>
#include <network/HealthEvent.h>

#include "../components/SceneNodeComponent.h"
#include "../core/Game.h"

HealthEventHandler::HealthEventHandler(Game *game)
        : game(game) {
}

void HealthEventHandler::handle(const Event &e) const {
	const HealthEvent &evt = static_cast<const HealthEvent &>(e);

	Entity *player = game->getEntityManager().getEntity((game->getGameClient().getCurrentPlayerEntityId()));
	HealthComponent *health = player->getComponent<HealthComponent>();
	health->setAmount(evt.getAmount());
	game->getGameGUIEventHandler()->handle(evt, health);
}
