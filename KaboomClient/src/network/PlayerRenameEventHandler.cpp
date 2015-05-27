#include "PlayerRenameEventHandler.h"

#include <core/Player.h>
#include <network/PlayerRenameEvent.h>

#include "../core/Game.h"

PlayerRenameEventHandler::PlayerRenameEventHandler(Game *game)
        : game(game) {
}

void PlayerRenameEventHandler::handle(const Event &e) const {
    const PlayerRenameEvent &evt = static_cast<const PlayerRenameEvent &>(e);
	std::cout << "i am in player rename event handler" << std::endl;
    Player *player = game->getPlayers().at(evt.getPlayerId());
    player->setName(evt.getName());
	game->getGameGUIEventHandler()->handle(evt, player);
}
