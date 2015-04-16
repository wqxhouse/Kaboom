#include "PlayerInputEventHandler.h"

#include <network/PlayerInputEvent.h>

#include "../core/ServerPlayer.h"

PlayerInputEventHandler::PlayerInputEventHandler(Game *game) {
}

PlayerInputEventHandler::~PlayerInputEventHandler() {
}

void PlayerInputEventHandler::handle(const Event &e) const {
    const PlayerInputEvent &evt = static_cast<const PlayerInputEvent &>(e);

    Entity *entity = game->getEntityManager().getEntity(evt.getPlayerId());
    ServerPlayer *player = static_cast<ServerPlayer *>(entity);

    if (evt.isMovingForward()) {
        player->setVelocityY(1);
    } else if (evt.isMovingBackward()) {
        player->setVelocityY(-1);
    } else {
        player->setVelocityY(0);
    }

    if (evt.isMovingLeft()) {
        player->setVelocityX(-1);
    } else if (evt.isMovingRight()) {
        player->setVelocityX(1);
    } else {
        player->setVelocityX(0);
    }
}
