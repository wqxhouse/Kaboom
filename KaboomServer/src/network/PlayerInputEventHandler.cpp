#include "PlayerInputEventHandler.h"

#include <components/PositionComponent.h>
#include <components/RotationComponent.h>
#include <core/Entity.h>
#include <network/PlayerInputEvent.h>

#include "../components/CharacterRotationComponent.h"
#include "../components/InputComponent.h"
#include "../core/Game.h"
#include "../math/util.h"

PlayerInputEventHandler::PlayerInputEventHandler(Game *game)
        : game(game) {
}

void PlayerInputEventHandler::handle(const Event &e) const {
    const PlayerInputEvent &evt = static_cast<const PlayerInputEvent &>(e);

    Entity *player = game->getEntityManager().getEntity(evt.getPlayerId());

    InputComponent *inputComp = player->getComponent<InputComponent>();
    inputComp->setMovingForward(evt.isMovingForward());
    inputComp->setMovingBackward(evt.isMovingBackward());
    inputComp->setMovingLeft(evt.isMovingLeft());
    inputComp->setMovingRight(evt.isMovingRight());
    inputComp->setJumping(evt.isJumping());
    inputComp->setAttacking1(evt.isAttacking1());
    inputComp->setAttacking2(evt.isAttacking2());

    RotationComponent *rotComp = player->getComponent<RotationComponent>();
    rotComp->setRotation(euler2Quat(evt.getYaw(), 0.0f, 0.0f));

    CharacterRotationComponent *charRotComp = player->getComponent<CharacterRotationComponent>();
    charRotComp->setRotation(euler2Quat(evt.getYaw(), evt.getPitch(), 0.0f));
}
