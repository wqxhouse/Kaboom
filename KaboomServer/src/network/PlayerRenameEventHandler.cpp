#include "PlayerRenameEventHandler.h"

#include <components/EquipmentComponent.h>
#include <core/EntityManager.h>
#include <core/Player.h>
#include <network/PlayerRenameEvent.h>

#include "../core/Game.h"

PlayerRenameEventHandler::PlayerRenameEventHandler(Game *game)
        : game(game) {
}

void PlayerRenameEventHandler::handle(const Event &e) const {
    const PlayerRenameEvent &evt = static_cast<const PlayerRenameEvent &>(e);

    Player *player = game->getPlayers().at(evt.getPlayerId());
    player->setName(evt.getName());

    game->getGameServer().sendPlayerRenameEvent(player);
}
