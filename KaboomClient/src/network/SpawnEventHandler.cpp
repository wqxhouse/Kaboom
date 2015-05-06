#include "SpawnEventHandler.h"

#include <core/EntityType.h>
#include <network/SpawnEvent.h>

#include "../core/Game.h"

SpawnEventHandler::SpawnEventHandler(Game *game)
        : game(game) {
}

void SpawnEventHandler::handle(const Event &e) const {
    const SpawnEvent &evt = static_cast<const SpawnEvent &>(e);

    EntityType type = evt.getType();
    Entity *entity = nullptr;

    if ((type & CAT_CHARACTER) == CAT_CHARACTER) {
        entity = game->getCharacterFactory().createCharacter(
                evt.getEntityId(),
                evt.getType(),
                evt.getX(),
                evt.getY(),
                evt.getZ(),
                evt.getYaw(),
                evt.getPitch());
    } else if ((type & CAT_BOMB) == CAT_BOMB) {
        entity = game->getBombFactory().createBomb(
                evt.getEntityId(),
                evt.getType(),
                evt.getX(),
                evt.getY(),
                evt.getZ(),
                evt.getYaw(),
                evt.getPitch());
    }

    if (entity != nullptr) {
        game->addEntity(entity);
    }
}
