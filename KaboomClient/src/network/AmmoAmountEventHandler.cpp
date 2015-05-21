#include "AmmoAmountEventHandler.h"

#include <osg/MatrixTransform>

#include <Core.h>
#include <GeometryObject.h>
#include <components/InventoryComponent.h>
#include <core/Entity.h>
#include <network/AmmoAmountEvent.h>

#include "../components/SceneNodeComponent.h"
#include "../core/Game.h"

AmmoAmountEventHandler::AmmoAmountEventHandler(Game *game)
        : game(game) {
}

void AmmoAmountEventHandler::handle(const Event &e) const {
	const AmmoAmountEvent &evt = static_cast<const AmmoAmountEvent &>(e);

	Entity *player = game->getEntityManager().getEntity((game->getGameClient().getCurrentPlayerEntityId()));
    InventoryComponent *invComp = player->getComponent<InventoryComponent>();
	//bombConCom->setAmount(evt.getAmount());
    game->getGameGUIEventHandler()->handle(evt, invComp);
}
