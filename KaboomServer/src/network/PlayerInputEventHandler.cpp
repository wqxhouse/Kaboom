#include "PlayerInputEventHandler.h"

#include <core/Entity.h>
#include <core/PositionComponent.h>
#include <core/RotationComponent.h>
#include <network/PlayerInputEvent.h>

#include "../core/Game.h"
#include "../core/InputComponent.h"

PlayerInputEventHandler::PlayerInputEventHandler(Game *game)
        : game(game) {
}

PlayerInputEventHandler::~PlayerInputEventHandler() {
}

void PlayerInputEventHandler::handle(const Event &e) const {
    const PlayerInputEvent &evt = static_cast<const PlayerInputEvent &>(e);

    Entity *player = game->getEntityManager().getEntity(evt.getPlayerId());

    InputComponent *inputCom = player->getComponent<InputComponent>();
    inputCom->setMovingForward(evt.isMovingForward());
    inputCom->setMovingBackward(evt.isMovingBackward());
    inputCom->setMovingLeft(evt.isMovingLeft());
    inputCom->setMovingRight(evt.isMovingRight());
    inputCom->setJumping(evt.isJumping());
    inputCom->setFiring(evt.isFiring());

	//TODO hard code for now, used for testing
	inputCom->setFireMode(FireMode::LEFT_CLICK);
	inputCom->setSelectedBombType(BombType::CRACKER_BOMB);

    RotationComponent *rotCom = player->getComponent<RotationComponent>();
    rotCom->setYaw(evt.getYaw());
    rotCom->setPitch(evt.getPitch());
}
