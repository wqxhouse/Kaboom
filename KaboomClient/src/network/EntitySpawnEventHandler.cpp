#include "EntitySpawnEventHandler.h"

#include <network/EntitySpawnEvent.h>

EntitySpawnEventHandler::EntitySpawnEventHandler(Game *game)
    : game(game) {
}

EntitySpawnEventHandler::~EntitySpawnEventHandler() {
}

void EntitySpawnEventHandler::handle(const Event &e) const {
	const EntitySpawnEvent &evt = static_cast<const EntitySpawnEvent &>(e);

	switch (evt.getType()) {
	case EntityType::PLAYER:
		printf("begin creation of player\n");
		std::cout << evt << std::endl;
		game->addSceneNodeEntity(game->getPlayerFactory().createPlayer(evt.getPlayerId(), evt.getX(), evt.getY(), evt.getZ()));
		printf("done with creation of player");
		break;
	case EntityType::BOMB:
		game->addSceneNodeEntity(game->getBombFactory().createBomb(evt.getPlayerId(), evt.getX(), evt.getY(), evt.getZ()));
		printf("Creating a bomb");
		break;
	case EntityType::UNINITIATED:
		printf("Error in entity type");
		break;
	}
	


}
