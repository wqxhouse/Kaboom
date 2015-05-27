#include "PlayerStatusEventHandler.h"

#include <components/PlayerStatusComponent.h>
#include "../components/ModelComponent.h"
#include <core/Entity.h>
#include <network/PlayerStatusEvent.h>

#include "../core/Game.h"

PlayerStatusEventHandler::PlayerStatusEventHandler(Game *game)
        : game(game) {
}

void PlayerStatusEventHandler::handle(const Event &e) const {
    const PlayerStatusEvent &evt = static_cast<const PlayerStatusEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getEntityId());

    auto playerStatusComp = entity->getComponent<PlayerStatusComponent>();
    playerStatusComp->setAlive(evt.isAlive());
    playerStatusComp->setRunning(evt.isRunning());
    playerStatusComp->setJumping(evt.isJumping());
    playerStatusComp->setAttacking(evt.isAttacking());
    playerStatusComp->setDamaged(evt.isDamaged());

	auto modelComp = entity->getComponent<ModelComponent>();
	Model* model = modelComp->getModel();

	if (evt.isRunning()) {
		model->playAnimation(RUNNING);
	}
	else {
		model->playAnimation(IDLE);
	}
}
