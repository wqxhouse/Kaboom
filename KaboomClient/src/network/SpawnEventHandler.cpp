#include "SpawnEventHandler.h"

#include <core/EntityType.h>
#include <network/SpawnEvent.h>
#include <osg/Group>
#include "../core/Game.h"
#include "../core/SceneNodeComponent.h"

SpawnEventHandler::SpawnEventHandler(Game *game)
    : game(game) {
} 

SpawnEventHandler::~SpawnEventHandler() {
}

void SpawnEventHandler::handle(const Event &e) const {
    const SpawnEvent &evt = static_cast<const SpawnEvent &>(e);
	Entity * entity;
	SceneNodeComponent * sceneNode;
	switch (evt.getType()) {
	case EntityType::PLAYER:
		printf("begin creation of player\n");
		std::cout << evt << std::endl;


		entity = game->getPlayerFactory().createPlayer(evt.getEntityId(), evt.getX(), evt.getY(), evt.getZ());
		
		sceneNode = entity->getComponent<SceneNodeComponent>();
		game->getGeometryManager()->addGeometry(std::to_string(static_cast<int>(entity->getId())),
												sceneNode->getNode(),
												osg::Vec3(evt.getX(), evt.getY(), evt.getZ()));

		printf("done with creation of player");
		break;
	case EntityType::BOMB:

		entity = game->getBombFactory().createBomb(evt.getEntityId(), evt.getX(), evt.getY(), evt.getZ());
		sceneNode = entity->getComponent<SceneNodeComponent>();
		game->getGeometryManager()->addGeometry(std::to_string(static_cast<int>(entity->getId())),
			sceneNode->getNode(),
			osg::Vec3(evt.getX(), evt.getY(), evt.getZ()));
		printf("Creating a bomb");
		break;
	case EntityType::UNINITIATED:
		printf("Error in entity type");
		break;
	}
}
