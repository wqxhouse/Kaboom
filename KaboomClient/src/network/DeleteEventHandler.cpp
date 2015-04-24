#include "DeleteEventHandler.h"

#include <core/EntityType.h>
#include <network/DeleteEvent.h>

#include "../core/Game.h"

DeleteEventHandler::DeleteEventHandler(Game *game)
    : game(game) {
}

DeleteEventHandler::~DeleteEventHandler() {
}

void DeleteEventHandler::handle(const Event &e) const {
	const DeleteEvent &evt = static_cast<const DeleteEvent &>(e);
	game->getEntityManager().destroyEntity(evt.getEntityId());
	game->getGeometryManager()->deleteGeometry(std::to_string(static_cast<int>(evt.getEntityId())));
}
