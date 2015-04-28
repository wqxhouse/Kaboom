#include "ExplosionEventHandler.h"

#include <network/ExplosionEvent.h>

#include "../core/Game.h"

ExplosionEventHandler::ExplosionEventHandler(Game *game)
        : game(game) {
}

ExplosionEventHandler::~ExplosionEventHandler() {
}

void ExplosionEventHandler::handle(const Event &e) const {
    const ExplosionEvent &evt = static_cast<const ExplosionEvent &>(e);

    EntityManager &entityManager = game->getEntityManager();
    Entity *bomb = entityManager.getEntity(evt.getBombId());

    // TODO: Handle explosion effect.

    game->getGeometryManager()->deleteGeometry(std::to_string(evt.getBombId()));

    entityManager.destroyEntity(bomb->getId());
}
