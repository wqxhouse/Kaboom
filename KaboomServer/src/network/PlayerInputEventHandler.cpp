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

    // TODO: Handle player input
}
