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

    Entity *entity = game->getCurrentPlayer()->getEntity();
    HealthComponent *health = entity->getComponent<HealthComponent>();
	if (health->getAmount() > evt.getAmount())
	{
		game->healthChanged = true;
		game->getGameGUIEventHandler()->damageScreen(true);
		game->damageTime = std::chrono::high_resolution_clock::now();
	}
	health->setAmount(evt.getAmount());
	game->getGameGUIEventHandler()->handle(evt, health);
}
