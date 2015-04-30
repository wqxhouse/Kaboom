#include "SpawnEventHandler.h"

#include <core/EntityType.h>
#include <network/SpawnEvent.h>

#include "../core/Game.h"

SpawnEventHandler::SpawnEventHandler(Game *game)
        : game(game) {
}

void SpawnEventHandler::handle(const Event &e) const {
    const SpawnEvent &evt = static_cast<const SpawnEvent &>(e);

    switch (evt.getType()) {
        case EntityType::PLAYER: {
            Entity *entity = game->getPlayerFactory().createPlayer(evt.getEntityId(), evt.getX(), evt.getY(), evt.getZ());
            game->addEntity(entity);
            break;
        }
        case EntityType::BOMB: {
            Entity *entity = game->getBombFactory().createBomb(evt.getEntityId(), evt.getX(), evt.getY(), evt.getZ());
            game->addEntity(entity);
            break;
        }
        case EntityType::UNINITIATED: {
            printf("Error in entity type\n");
            break;
        }
    }
}
