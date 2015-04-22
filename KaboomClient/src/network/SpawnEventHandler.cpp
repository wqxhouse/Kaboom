#include "SpawnEventHandler.h"

#include <core/EntityType.h>
#include <network/SpawnEvent.h>

#include "../core/Game.h"

SpawnEventHandler::SpawnEventHandler(Game *game)
    : game(game) {
}

SpawnEventHandler::~SpawnEventHandler() {
}

void SpawnEventHandler::handle(const Event &e) const {
    const SpawnEvent &evt = static_cast<const SpawnEvent &>(e);

	switch (evt.getType()) {
	case EntityType::PLAYER:
		printf("begin creation of player\n");
		std::cout << evt << std::endl;
		game->addSceneNodeEntity(game->getPlayerFactory().createPlayer(evt.getEntityId(), evt.getX(), evt.getY(), evt.getZ()));
		printf("done with creation of player");
		break;
	case EntityType::BOMB:
        game->addSceneNodeEntity(game->getBombFactory().createBomb(evt.getEntityId(), evt.getX(), evt.getY(), evt.getZ()));
		printf("Creating a bomb");
		break;
	case EntityType::UNINITIATED:
		printf("Error in entity type");
		break;
	}
}
